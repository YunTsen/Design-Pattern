#include "update_path_visitor.h"
#include "folder.h"
#include "file.h"
#include "link.h"
#include "node.h"

UpdatePathVisitor::UpdatePathVisitor(){}

void UpdatePathVisitor::visitFolder(Folder *folder){
    Iterator *it = folder->createIterator();
    for(it->first();!it->isDone();it->next()){
        it->currentItem()->setPath(folder);
        it->currentItem()->accept(this);
    }
}
void UpdatePathVisitor::visitFile(File *file){}

void UpdatePathVisitor::visitLink(Link * link){}