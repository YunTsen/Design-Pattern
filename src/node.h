#ifndef NODE_H
#define NODE_H
#include <stdio.h>
#include <sys/stat.h>
#include <iostream>
#include <regex>
#include "find_visitor.h"
#include "iterator.h"
#include "update_path_visitor.h"
class Node {
   public:
    Node(std::string path) : _path(path) {
        //tell if node exists
        //On success, zero is returned. On error, -1 is returned
        struct stat statbuf;
        if (lstat(_path.c_str(), &statbuf) == -1) {
            throw std::string("Node is not exist!");
        }

        //initiliz _name
        this->setName();
    }
    std::string name() {
        return _name;
    }
    std::string path() {
        return _path;
    }
    virtual void addChild(Node *chlid) {
        throw std::string("File could not addChlid.");
    }
    virtual Node *getChild(int num) {
        throw std::string("File could not getChlid.");
    }
    virtual std::string listNode() {
        throw std::string("File could not listNode.");
    }
    virtual bool isFolder() {
        return 0;
    }
    // virtual bool isLink(){
    //     return 0;
    // }
    virtual std::string findNode(std::string findName) {
        if (findName == this->_name)
            return this->_name;
        else
            return "";
    }
    virtual std::string findNodeInChildNode(std::string findName) {
        if (findName == this->_name)
            return this->_name;
        else
            return "";
    }

    virtual Iterator *createIterator() =0;

    virtual Node* getSource(){}

    std::string getPath() {
        return this->_path;
    }

    virtual void accept(FindVisitor *fvs) {  // accept visitor
    }
    virtual void accept(UpdatePathVisitor *upvs) {  // accept visitor
    }

    virtual void renameNode(std::string newName) {  //also set _name
        std::string newPath;
        std::regex ex(_name);
        newPath = std::regex_replace(_path, ex, newName);
        rename(_path.c_str(), newPath.c_str());
        _name = newName;
        _path = newPath;
    }

    virtual void setPath(Node *parentNode) {
        std::string parentPath = parentNode->getPath();
        _path = parentPath.append("/" + _name);
    }

   private:
    std::string _path, _name;
    void setName() {
        std::regex ex(".*/(.*)");
        std::smatch sm;
        //path with slash
        if (std::regex_match(_path, sm, ex)) {
            _name = sm[1];
        }
        //path without slash
        else {
            _name = _path;
        }
    }
};

#endif