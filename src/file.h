#ifndef FILE_H
#define FILE_H

#include <regex>
#include <string>
#include "node.h"
#include "null_iterator.h"

class File : public Node {
   public:
    File(std::string path) : Node(path) {
        //thow errMsg when the node is not a file
        struct stat statbuf;
        lstat(path.c_str(), &statbuf);
        if (!S_ISREG(statbuf.st_mode)) {
            throw std::string("It is not File!");
        }
    }
    void accept(FindVisitor *fvs){// accept visitor
        fvs->visitFile(this);
    } 
    void accept(UpdatePathVisitor *upvs){// accept visitor
        upvs->visitFile(this);
    } 
    Iterator *createIterator() {
        return new NullIterator;
    }
};

#endif