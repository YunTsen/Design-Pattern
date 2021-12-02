#include "filesystem_builder.h"
#include "file.h"
#include "folder.h"
#include "link.h"

#include <dirent.h>
#include <sys/stat.h>
#include <iostream>

//非const的static成員要在類別定義區塊之外初始化
FileSystemBuilder* FileSystemBuilder::_instance = new FileSystemBuilder;

FileSystemBuilder::FileSystemBuilder() {}

FileSystemBuilder* FileSystemBuilder::instance() {
    return _instance;
}

void FileSystemBuilder::build(std::string path) {
    DIR* dir = opendir(path.c_str());
    struct dirent* entry;
    struct stat statbuf;
    lstat(path.c_str(), &statbuf);

    //build a file
    if (S_ISREG(statbuf.st_mode)) {
        _root = new File(path);
        return;
    }

    //build a Link
    else if (S_ISLNK(statbuf.st_mode)) {
        _root = new Link(path);
        return;
    }
    
    //build a folder
    else {
        _root = new Folder(path);
        while ((entry = readdir(dir)) != NULL) {
            //for folder
            FileSystemBuilder* fb = new FileSystemBuilder;
            //for link
            DIR* dirChild;
            struct stat statbuf;
            char buf[PATH_MAX];
            char* exist;
            Node* representedNode;
            std::string rp;

            if ((entry->d_name) == std::string(".") || (entry->d_name) == std::string(".."))
                continue;
            //std::cout<<entry->d_name<<std::endl;
            std::string childPath = path + "/" + entry->d_name;
            switch (entry->d_type) {
                case DT_DIR:
                    //std::cout<<"DIR"<<std::endl;
                    fb->build(childPath);
                    _root->addChild(fb->getRoot());
                    break;
                case DT_REG:
                    //std::cout<<"REG"<<std::endl;
                    _root->addChild(new File(childPath));
                    break;
                case DT_LNK:
                    exist = realpath(childPath.c_str(), buf);
                    rp = exist;
                    //std::cout<<rp<<std::endl;
                    stat(childPath.c_str(), &statbuf);
                    if (S_ISREG(statbuf.st_mode)) {
                        representedNode = new File(rp);
                        _root->addChild(new Link(childPath, representedNode));
                    } else if (S_ISDIR(statbuf.st_mode)) {
                        representedNode = new Folder(rp);
                        _root->addChild(new Link(childPath, representedNode));
                    }
                    break;
            }
        }
    }
}

Node* FileSystemBuilder::getRoot() {
    return _root;
}