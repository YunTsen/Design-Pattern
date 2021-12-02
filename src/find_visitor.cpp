#include "find_visitor.h"
#include "file.h"
#include "folder.h"
#include "node.h"
#include "link.h"
#include "utilities.h"
#include <string>

FindVisitor::FindVisitor(std::string name):_name(name){
}

void FindVisitor::visitFolder(Folder* folder){
    _node=folder;
}

void FindVisitor::visitFile(File* file){
    _node=file;
}

void FindVisitor::visitLink(Link* link){
    // You only need to check the link itself,and you don't have to check the node that the link point to!

}

std::string FindVisitor::findResult(){
    Utilities *utilities=new Utilities;
    return utilities->findNode(_node,_name);
}