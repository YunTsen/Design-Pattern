#include <dirent.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <iostream>
#include <regex>
#include <string>
#include "../src/file.h"
#include "../src/filesystem_builder.h"
#include "../src/find_visitor.h"
#include "../src/folder.h"
#include "../src/link.h"
#include "../src/node.h"

class VisitorTest : public ::testing::Test {
   protected:
    Node *test_folder;
    Node *hello_txt;
    Node *hw;
    Node *hw_hw;
    Node *hello_txt_In_hw;
    Node *hello_txt_In_hw_hw;
    void SetUp() {
        test_folder = new Folder("test/test_folder");
        hello_txt = new File("test/test_folder/hello.txt");
        hw = new Folder("test/test_folder/hw");
        hw_hw = new Folder("test/test_folder/hw/hw");
        hello_txt_In_hw = new File("test/test_folder/hw/hello.txt");
        hello_txt_In_hw_hw = new File("test/test_folder/hw/hw/hello.txt");
        test_folder->addChild(hello_txt);
        test_folder->addChild(hw);
        hw->addChild(hello_txt_In_hw_hw);
        hw->addChild(hw_hw);
        hw_hw->addChild(hello_txt_In_hw_hw);
    }
    void TearDown() {
        delete test_folder;
        delete hello_txt;
        delete hw;
        delete hw_hw;
        delete hello_txt_In_hw;
        delete hello_txt_In_hw_hw;
    }
};

TEST_F(VisitorTest, FindVisitor_VisitFolder) {
    FindVisitor *fvs = new FindVisitor("hello.txt");
    test_folder->accept(fvs);
    ASSERT_EQ("./hello.txt\n./hw/hello.txt\n./hw/hw/hello.txt", fvs->findResult());
}

TEST_F(VisitorTest, FindVisitor_VisitFile) {
    FindVisitor *fvs = new FindVisitor("hello.txt");
    hello_txt->accept(fvs);
    ASSERT_EQ("hello.txt", fvs->findResult());
}

TEST_F(VisitorTest, NewPath) {
    std::string oldPath = hw->getPath();
    std::regex ex(hw->name());
    ASSERT_EQ("test/test_folder/hwRe", std::regex_replace(oldPath, ex, "hwRe"));
    ASSERT_EQ("test/test_folder/hw", oldPath);
}

TEST_F(VisitorTest, Node_RenameForFolder) {
    hw->renameNode("hwRe");
    ASSERT_EQ("hwRe", hw->name());
    struct stat st;
    if (stat("test/test_folder/hwRe", &st) == -1) {
        ASSERT_TRUE(false);
    }
    hw->renameNode("hw");
}

TEST_F(VisitorTest, Node_RenameForFile) {
    hello_txt->renameNode("helloRe.txt");
    ASSERT_EQ("helloRe.txt", hello_txt->name());
    struct stat st;
    if (stat("test/test_folder/helloRe.txt", &st) == -1) {
        ASSERT_TRUE(false);
    }
    hello_txt->renameNode("hello.txt");
}

TEST_F(VisitorTest, Node_SetPathReferenceParentNode) {
    hw_hw->setPath(test_folder);
    ASSERT_EQ("test/test_folder/hw", hw_hw->getPath());
}

TEST_F(VisitorTest, UpdatePathVisitor) {
    hw->renameNode("hwRe");
    UpdatePathVisitor *upvs = new UpdatePathVisitor;
    hw->accept(upvs);
    ASSERT_EQ("test/test_folder/hwRe", hw->getPath());
    ASSERT_EQ("test/test_folder/hwRe/hw", hw_hw->getPath());
    hw->renameNode("hw");
}

TEST_F(VisitorTest, CreateLink) {
    ASSERT_ANY_THROW(Node *link = new Link("test/test_folder/hello.txt", hello_txt));
    try {
        Node *link = new Link("test/test_folder", test_folder);
    } catch (std::string errMsg) {
        ASSERT_EQ("It is not Link!", errMsg);
    }
}

TEST_F(VisitorTest, LinkName) {
    Link *hw_lnk = new Link("test/hw_lnk", hw);
    ASSERT_EQ("hw_lnk", hw_lnk->name());
}

TEST_F(VisitorTest, Link_Rename) {
    Link *hw_lnk = new Link("test/hw_lnk", hw);
    hw_lnk->renameNode("hw_lnkRe");
    ASSERT_EQ("hw_lnkRe", hw_lnk->name());
    hw_lnk->renameNode("hw_lnk");

    Link *hellotxt_lnk = new Link("test/hellotxt_lnk", hw);
    hellotxt_lnk->renameNode("hellotxt_lnkRe");
    ASSERT_EQ("hellotxt_lnkRe", hellotxt_lnk->name());
    hellotxt_lnk->renameNode("hellotxt_lnk");
}

TEST_F(VisitorTest, Link_GetSource) {
    Link *hw_lnk = new Link("test/hw_lnk", hw);
    Node *node = hw_lnk->getSource();
    ASSERT_EQ(hw, node);
}

TEST(FileSystemBuilderTest, BuildFile) {
    FileSystemBuilder *fb = FileSystemBuilder::instance();
    fb->build("test/test_folder/hello.txt");
    ASSERT_EQ("hello.txt", fb->getRoot()->name());
    ASSERT_EQ("test/test_folder/hello.txt", fb->getRoot()->getPath());
}

TEST(FileSystemBuilderTest, BuildFolder) {
    FileSystemBuilder *fb = FileSystemBuilder::instance();
    fb->build("test");
    //ASSERT_EQ("test_folder", fb->getRoot()->name());
    //ASSERT_EQ("test/test_folder", fb->getRoot()->getPath());
    
    Iterator *it=fb->getRoot()->createIterator();
    for(it->first();!it->isDone();it->next()){
        std::cout<<it->currentItem()->name()<<std::endl;
    }
    it->first();
    it->next();
    it->next();
    it->next();
    std::cout<<it->currentItem()->isFolder()<<std::endl;
    it->next();
    std::cout<<it->currentItem()->name()<<std::endl;
    std::cout<<it->currentItem()->isFolder()<<std::endl;
    Iterator *it2=it->currentItem()->createIterator();
    for(it2->first();!it2->isDone();it2->next()){
        std::cout<<it2->currentItem()->name()<<std::endl;
    }
}

TEST(FileSystemBuilderTest, BuildLink) {
    FileSystemBuilder *fb = FileSystemBuilder::instance();
    fb->build("test/hw_lnk");
    ASSERT_EQ("hw_lnk", fb->getRoot()->name());
    ASSERT_EQ("test/hw_lnk", fb->getRoot()->getPath());
    ASSERT_EQ(nullptr,fb->getRoot()->getSource());
}

// TEST(FileSystemBuilderTest, Singleton){
//   FileSystemBuilder * fb = FileSystemBuilder::instance();
// }

TEST(LinkTest,NoTarget){
    Link *link=new Link("test/hw_lnk");
    ASSERT_EQ(nullptr,link->getSource());
}

TEST(CApi, LnkTest) {
    struct stat statbuf;
    lstat("test/hw_lnk", &statbuf);
    ASSERT_TRUE(S_ISLNK(statbuf.st_mode));
    ASSERT_FALSE(S_ISREG(statbuf.st_mode));
    ASSERT_FALSE(S_ISDIR(statbuf.st_mode));
    char buf[PATH_MAX];
    char *exist; 
    exist=realpath("test/hw_lnk",buf);
    //std::cout<<(exist==NULL)<<exist<<std::endl;//0/mnt/d/Projects/108598056_hw/test/test_folder/hw
    std::string s(exist);
    //rp=resolved_path;
    //ASSERT_EQ("/mnt/d/Projects/108598056_hw/test/test_folder/hw",s);
    stat("test/hw_lnk", &statbuf);
    ASSERT_FALSE(S_ISLNK(statbuf.st_mode));
    ASSERT_FALSE(S_ISREG(statbuf.st_mode));
    ASSERT_TRUE(S_ISDIR(statbuf.st_mode));
}

TEST(CApi, Opendir) {
    //node do not exist
    DIR *dir = opendir("notExist");
    ASSERT_EQ(nullptr, dir);

    //open file
    dir = opendir("test/test_folder/hello.txt");
    ASSERT_EQ(nullptr, dir);

    //open link to file
    dir = opendir("test/hellotxt_lnk");
    ASSERT_EQ(nullptr, dir);

    //open link to folder
    dir = opendir("test/hw_lnk");
    ASSERT_NE(nullptr, dir);

    //open folder
    dir = opendir("test");
    ASSERT_NE(nullptr, dir);

    // struct dirent *entry;
    // // while((entry=readdir(dir))!=NULL){}
    // entry=readdir(dir);
    // EXPECT_EQ(std::string("."),entry->d_name);
    // entry=readdir(dir);
    // EXPECT_EQ(std::string(".."),entry->d_name);
    // entry=readdir(dir);
    // EXPECT_EQ(std::string("hellotxt_link.lnk"),entry->d_name);
    // //ASSERT_TRUE(entry->d_type==DT_LNK); //FALSE
    // //ASSERT_FALSE(entry->d_type==DT_REG); //TRUE
    // entry=readdir(dir);
    // EXPECT_EQ(std::string("hellotxt_lnk"),entry->d_name);
    // ASSERT_TRUE(entry->d_type==DT_LNK);
    // ASSERT_FALSE(entry->d_type==DT_REG);
    // entry=readdir(dir);
    // EXPECT_EQ(std::string("hw_link.lnk"),entry->d_name);
    // entry=readdir(dir);
    // EXPECT_EQ(std::string("hw_lnk"),entry->d_name);
    // entry=readdir(dir);
    // EXPECT_EQ(std::string("test_folder"),entry->d_name);
    // ASSERT_TRUE(entry->d_type==DT_DIR);
    // ASSERT_FALSE(entry->d_type==DT_REG);
    // entry=readdir(dir);
    // EXPECT_EQ(std::string("ut_fs.h"),entry->d_name);
    // entry=readdir(dir);
    // EXPECT_EQ(std::string("ut_main.cpp"),entry->d_name);
    // entry=readdir(dir);
    // EXPECT_EQ(std::string("ut_node.h"),entry->d_name);
}
