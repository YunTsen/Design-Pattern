#ifndef LINK_H
#define LINK_H
#include "find_visitor.h"
#include "update_path_visitor.h"
#include "null_iterator.h"
#include "node.h"
#include <string>
#include <sys/stat.h>

class Link:public Node{
public:
  //constructor that is given the target
  Link(std::string path, Node * node): Node(path){
    // You should check path if it is not a link that you should throw string "It is not Link!"
    struct stat sb;
    int linkExist = lstat(this->getPath().c_str(), &sb);
    if(linkExist==-1||!S_ISLNK(sb.st_mode)){
      throw std::string("It is not Link!");
    }
    _node =node;
  }
  
  //constructor that is NOT given the target
  Link(std::string path): Node(path){
    // You should check path if it is not a link that you should throw string "It is not Link!"
    struct stat sb;
    int linkExist = lstat(this->getPath().c_str(), &sb);
    if(linkExist==-1||!S_ISLNK(sb.st_mode)){
      throw std::string("It is not Link!");
    }
    _node =nullptr;
  } 

  void accept(FindVisitor *fvs){
    // accept visitor
    _node->accept(fvs);
  } 
  
  void accept(UpdatePathVisitor *upvs){
    // accept visitor
    _node->accept(upvs);
  } 

  void addLink(Node * node){
     // add symbolic link
     _node=node;
  }

  Node * getSource(){
    return _node;
  } // return node that the link point to!

  Iterator* createIterator(){
    return new NullIterator;
  } // return null Iterator

  // bool isLink(){
    
  // }

private:
  Node *_node;

};

#endif