#ifndef FOLDER_H
#define FOLDER_H

#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include "iterator.h"
#include "node.h"
#include "find_visitor.h"
#include "update_path_visitor.h"

class Folder : public Node {
   public:
    class FolderIterator : public Iterator {
       public:
        FolderIterator( Folder *f) : _f(f) {}
        void first() {
            // initialization
            _current = _f->_childNodes.begin();
        }
         Node *currentItem() {
            // if iterator is to the end that it should throw string "No current item!"
            // if iterator is not to the end that it should return current node
            if (this->isDone()) {
                throw std::string("No current item!");
            } else {
                return _current->second;
            }
        }
        void next() {
            // if iterator is to the end that it should throw string "Moving past the end!"
            // if iterator is not to the end that it should add 1
            if (this->isDone()) {
                throw std::string("Moving past the end!");
            } else {
                _current++;
            }
        }
        bool isDone() {
            // return iterator is to the end or not?
            return _current == _f->_childNodes.end();
        }

       private:
         Folder *_f;
        std::map<std::string, Node *>::iterator _current;
    };

    Folder(std::string path) : Node(path) {
        //thow errMsg when the node is not a folder
        struct stat statbuf;
        stat(path.c_str(), &statbuf);
        if (!S_ISDIR(statbuf.st_mode)) {
            throw std::string("It is not Folder!");
        }
    }
    void addChild(Node *childNode) {
        _childNodes[childNode->name()] = childNode;
    }
    // Node *getChild(int num)
    // {
    //     return _childNodes[num];
    // }
    std::string listNode() {
        ////***Folder First then File version***////
        // std::vector<std::string> childFolders;
        // std::vector<std::string> childFiles;
        // for(std::vector<Node*>::iterator it=_childNodes.begin();it!=_childNodes.end();it++){
        //     if((*it)->isFolder()){
        //         childFolders.push_back((*it)->name());
        //     }
        //     else{
        //         childFiles.push_back((*it)->name());
        //     }
        // }
        // std::sort(childFolders.begin(),childFolders.end());
        // std::sort(childFiles.begin(),childFiles.end());

        // std::stringstream ss;
        // for(std::vector<std::string>::iterator it = childFolders.begin();it!=childFolders.end();it++){
        //     ss<<(*it)<<" ";
        // }
        // for(std::vector<std::string>::iterator it = childFiles.begin();it!=childFiles.end();it++){
        //     ss<<(*it)<<" ";
        // }
        // std::string output = ss.str();
        // output.erase(output.end()-1);
        // return output;
        ////***Folder First then File version***////

        ////***No matter if it is a File or a Folder version***////
        std::vector<std::string> childNodes_names;
        for (std::map<std::string, Node *>::iterator it = _childNodes.begin(); it != _childNodes.end(); it++) {
            childNodes_names.push_back(it->second->name());
        }
        std::sort(childNodes_names.begin(), childNodes_names.end());
        std::stringstream ss;
        for (std::vector<std::string>::iterator it = childNodes_names.begin(); it != childNodes_names.end(); it++) {
            ss << (*it) << " ";
        }
        std::string output = ss.str();
        output.erase(output.end() - 1);

        return output;
        ////***No matter if it is a File or a Folder version***////
    }
    bool isFolder() {
        return 1;
    }
    std::string findNode(std::string findName) {
        std::stringstream ss;
        for (std::map<std::string, Node *>::iterator it = _childNodes.begin(); it != _childNodes.end(); it++) {
            std::string tem;
            tem = it->second->findNodeInChildNode(findName);
            if (tem == "") {
            } else {
                ss << "./" << tem << "\n";
            }
        }
        std::string output = ss.str();
        if (!output.empty())
            output.erase(output.end() - 1);
        return output;
    }
    //RecursionMethod
    std::string findNodeInChildNode(std::string findName) {
        std::stringstream ss;
        if (this->name() == findName) {
            ss << this->name() << "\n";
        }
        for (std::map<std::string, Node *>::iterator it = _childNodes.begin(); it != _childNodes.end(); it++) {
            std::string tem;
            tem = it->second->findNodeInChildNode(findName);
            if (tem == "") {
            } else {
                ss << this->name() << "/" << tem << "\n";
            }
        }
        std::string output = ss.str();
        if (!output.empty())
            output.erase(output.end() - 1);

        std::regex ex("\n");
        output = std::regex_replace(output, ex, "\n./");

        return output;
    }
    Iterator *createIterator() {
        return new FolderIterator(this);
    };
    ~Folder() {
        std::map<std::string, Node *>().swap(_childNodes);//或者_childNodes.swap(std::map<std::string, Node *>())；
    }
    void accept(FindVisitor *fvs) {  // accept visitor
        fvs->visitFolder(this);
    }
    void accept(UpdatePathVisitor *upvs) {  // accept visitor
        upvs->visitFolder(this);
    }

   private:
    std::map<std::string, Node *> _childNodes;
};

#endif