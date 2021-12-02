#ifndef FILESYSTEM_BUILDER_H
#define FILESYSTEM_BUILDER_H

#include <string>
class Node;

class FileSystemBuilder {
   protected:
    FileSystemBuilder();

   public:
    static FileSystemBuilder* instance();
    void build(std::string path);
    Node *getRoot();

   private:
   Node* _root;
   static FileSystemBuilder* _instance;//非const的static成員要在類別定義區塊之外初始化
};

#endif