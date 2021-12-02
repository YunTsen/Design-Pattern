#include <gtest/gtest.h>
#include <sys/stat.h>
#include <algorithm>
#include <iostream>
#include <map>
#include "../src/file.h"
#include "../src/folder.h"
#include "../src/utilities.h"

class FolderTest : public ::testing::Test {
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
class NodeTest : public ::testing::Test {
   protected:
    Node *hello_txt;
    Node *test_folder;
    void SetUp() {
        hello_txt = new File("test/test_folder/hello.txt");
        test_folder = new Folder("test/test_folder");
    }
    void TearDown() {
        delete hello_txt;
        delete test_folder;
    }
};
class UtilitiesTest : public ::testing::Test {
   protected:
    Node *test_folder;
    Node *hello_txt;
    Node *hw;
    Node *hw_hw;
    Node *hello_txt_In_hw;
    Node *hello_txt_In_hw_hw;
    Utilities *utilities;
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

//On success, zero is returned. On error, -1 is returned
TEST(StatTest, FileExist) {
    struct stat statbuf;
    ASSERT_EQ(0, stat("test/test_folder/hello.txt", &statbuf));
    ASSERT_EQ(0, stat("makefile", &statbuf));
    ASSERT_EQ(-1, stat("test/test_folder/hello", &statbuf));
}
//On success, zero is returned. On error, -1 is returned
TEST(StatTest, FolderExist) {
    struct stat statbuf;
    ASSERT_EQ(0, stat("test/test_folder", &statbuf));
    ASSERT_EQ(-1, stat("test/test_folde", &statbuf));
}
//use the S_ISDIR macro on the st_mode field,
//or S_IFDIR
//to tell if a given path is a directory or a file
TEST(StatTest, FileOrDirectory) {
    struct stat statbuf;
    stat("test/test_folder", &statbuf);
    //ASSERT_NE(0,statbuf.st_mode & S_IFDIR);
    ASSERT_EQ(1, S_ISDIR(statbuf.st_mode));
    stat("test/test_folder/hello.txt", &statbuf);
    //ASSERT_EQ(0,statbuf.st_mode & S_IFDIR);
    ASSERT_EQ(0, S_ISDIR(statbuf.st_mode));
}

TEST(FileTest, FileName) {
    File *hello_txt = new File("test/test_folder/hello.txt");
    ASSERT_EQ("hello.txt", hello_txt->name());
    File *makefile = new File("makefile");
    ASSERT_EQ("makefile", makefile->name());
}

TEST_F(FolderTest, FolderName) {
    ASSERT_EQ("test_folder", test_folder->name());
}

TEST_F(NodeTest, NodeName) {
    ASSERT_EQ("hello.txt", hello_txt->name());
    ASSERT_EQ("test_folder", test_folder->name());
}

TEST_F(NodeTest, NodePath) {
    ASSERT_EQ("test/test_folder/hello.txt", hello_txt->path());
    ASSERT_EQ("test/test_folder", test_folder->path());
}

TEST_F(NodeTest, NodeDoesNotExist) {
    ASSERT_ANY_THROW(Node *Not_exist_file = new File("test/test_folder/Not_exist_file"));
    try {
        Node *node = new File("test/test_folder/Not_exist_file");
    } catch (std::string errMsg) {
        ASSERT_EQ("Node is not exist!", errMsg);
    }
}

TEST_F(NodeTest, AddChild) {
    try {
        hello_txt->addChild(hello_txt);
    } catch (std::string errMsg) {
        ASSERT_EQ("File could not addChlid.", errMsg);
    }
}

TEST_F(NodeTest, IsFolder) {
    ASSERT_TRUE(test_folder->isFolder());
    ASSERT_FALSE(hello_txt->isFolder());
}

TEST_F(NodeTest, FileFindItself) {
    Node *hello_txt = new File("test/test_folder/hello.txt");
    EXPECT_EQ("hello.txt", hello_txt->findNode("hello.txt"));
}

TEST(FileTest, NotAFile) {
    ASSERT_ANY_THROW(Node *test_folder = new File("test/test_folder"));
    try {
        Node *test_folder = new File("test/test_folder");
    } catch (std::string errMsg) {
        ASSERT_EQ("It is not File!", errMsg);
    }
}

TEST(SortTest, SortStrings) {
    std::string str = "sfeawegk12131";
    std::sort(str.begin(), str.end());
    ASSERT_EQ("11123aeefgksw", str);
    std::vector<std::string> strings;
    strings.push_back(std::string("pen"));
    strings.push_back(std::string("apple"));
    strings.push_back(std::string("concrete"));
    strings.push_back(std::string("cake"));
    std::sort(strings.begin(), strings.end());
    ASSERT_EQ("apple", strings[0]);
    ASSERT_EQ("cake", strings[1]);
    ASSERT_EQ("concrete", strings[2]);
    ASSERT_EQ("pen", strings[3]);
}

TEST_F(FolderTest, NotAFolder) {
    ASSERT_ANY_THROW(Node *hello_txt = new Folder("test/test_folder/hello.txt"));
    try {
        Node *hello_txt = new Folder("test/test_folder/hello.txt");
    } catch (std::string errMsg) {
        ASSERT_EQ("It is not Folder!", errMsg);
    }
}

// TEST_F(FolderTest, AddChild) {
//     ASSERT_EQ("hello.txt", test_folder->getChild(0)->name());
//     ASSERT_EQ("hw", test_folder->getChild(1)->name());
// }

TEST_F(FolderTest, ListNode) {
    ASSERT_EQ("hello.txt hw", test_folder->listNode());
}

TEST_F(FolderTest, FolderFindItself) {
    EXPECT_EQ("", test_folder->findNode("test_folder"));
}

TEST_F(FolderTest, FindSameNameFolderInFolder) {
    EXPECT_EQ("./hw\n./hw/hw", test_folder->findNode("hw"));
}

TEST_F(FolderTest, FindFilesInFolder) {
    EXPECT_EQ("./hello.txt\n./hw/hello.txt\n./hw/hw/hello.txt", test_folder->findNode("hello.txt"));
}

TEST(MapTest, First) {
    std::map<std::string, std::string> myMap;
    myMap["a"] = "first";
    myMap["c"] = "second";
    myMap["b"] = "third";
    // for(std::map<std::string,std::string>::iterator it = myMap.begin(); it!=myMap.end();it++){
    //     std::cout << it->first << std::endl;//a b c
    // }
}

TEST_F(NodeTest, createIterator) {
    Iterator *it = hello_txt->createIterator();
    try {
        it->first();
    } catch (std::string errMsg) {
        ASSERT_EQ("no child member", errMsg);
    }
    try {
        it->currentItem();
    } catch (std::string errMsg) {
        ASSERT_EQ("no child member", errMsg);
    }
    try {
        it->next();
    } catch (std::string errMsg) {
        ASSERT_EQ("no child member", errMsg);
    }
    ASSERT_TRUE(it->isDone());
}
TEST_F(NodeTest, GetPath){
    ASSERT_EQ("test/test_folder/hello.txt",hello_txt->getPath());
}

TEST_F(FolderTest, CreateIterator){
    Iterator *it = test_folder->createIterator();
    it->first();
    ASSERT_EQ(hello_txt,it->currentItem());
    it->next();
    ASSERT_EQ(hw,it->currentItem());
    ASSERT_FALSE(it->isDone());

    /*
    //用Iterator可以改變物件成員
    //除非currentItem()傳回型態改 Node*
    std::cout<<it->currentItem()->name()<<std::endl;//hw
    *(it->currentItem())=*hello_txt;
    std::cout<<it->currentItem()->name()<<std::endl;//hello.txt
    */

    it->next();
    ASSERT_TRUE(it->isDone());
    try{
        it->next();
    }catch(std::string errMsg){
        ASSERT_EQ("Moving past the end!",errMsg);
    }
    try{
        it->currentItem();
    }catch(std::string errMsg){
        ASSERT_EQ("No current item!",errMsg);
    }
}
TEST_F(UtilitiesTest,listNode){
    try{
        utilities->listNode(hello_txt);
    }catch(std::string errMsg){
        ASSERT_EQ("Not a directory", errMsg);
    }
    ASSERT_EQ("hello.txt hw", utilities->listNode(test_folder));
}
TEST_F(UtilitiesTest,findNode){
    ASSERT_EQ("hello.txt",utilities->findNode(hello_txt,"hello.txt"));
    ASSERT_EQ("", utilities->findNode(test_folder,"test_folder"));
    ASSERT_EQ("./hello.txt\n./hw/hello.txt\n./hw/hw/hello.txt", utilities->findNode(test_folder,"hello.txt"));

}
TEST_F(UtilitiesTest,DynamicCast){
    Folder *folder1=dynamic_cast<Folder*>(hello_txt);
    Folder *folder2 = dynamic_cast<Folder*>(hw);
    ASSERT_EQ(nullptr,folder1);
    ASSERT_NE(nullptr,folder2);
}