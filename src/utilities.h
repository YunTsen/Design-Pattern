#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include "folder.h"
#include "node.h"

class Utilities {
   public:
    std::string listNode(Node *node) {
        // You should use iterator pattern to access node in folder!
        // You can use dynamic_cast if you need
        // If node is file that it should throw string "Not a directory"
        // This function is same as Hw4 listNode()
        Folder *folder = dynamic_cast<Folder *>(node);
        if (folder) {
            std::vector<std::string> childNodes_names;
            Iterator *it = folder->createIterator();
            for (it->first(); !it->isDone(); it->next()) {
                childNodes_names.push_back(it->currentItem()->name());
            }
            std::sort(childNodes_names.begin(), childNodes_names.end());
            std::stringstream ss;
            for (std::vector<std::string>::iterator it = childNodes_names.begin(); it != childNodes_names.end(); it++) {
                ss << (*it) << " ";
            }
            std::string output = ss.str();
            output.erase(output.end() - 1);

            return output;
        } else {
            throw std::string("Not a directory");
        }
    }

    std::string findNode(Node *node, std::string name) {
        // You should use iterator pattern to access node in folder!
        // You can use dynamic_cast if you need
        // This function is same as Hw4 findNode()
        Folder *folder = dynamic_cast<Folder *>(node);
        if (folder) {
            std::stringstream ss;
            Iterator *it = folder->createIterator();
            for (it->first(); !it->isDone(); it->next()) {
                std::string tem;
                tem = this->findNodeInChildNode(it->currentItem(), name);
                if (tem == "") {
                } else {
                    ss << "./" << tem << "\n";
                }
            }
            std::string output = ss.str();
            if (!output.empty())
                output.erase(output.end() - 1);
            return output;
        } else {
            if (node->name() == name)
                return node->name();
        }
        return "";
    }

    std::string findNodeInChildNode( Node *node, std::string name) {
         Folder *folder = dynamic_cast< Folder *>(node);
        if (folder) {
            std::stringstream ss;
            if (folder->name() == name) {
                ss << folder->name() << "\n";
            }
            Iterator *it = folder->createIterator();
            for (it->first(); !it->isDone(); it->next()) {
                std::string tem;
                tem = this->findNodeInChildNode(it->currentItem(), name);
                if (tem == "") {
                } else {
                    ss << folder->name() << "/" << tem << "\n";
                }
            }
            std::string output = ss.str();
            if (!output.empty())
                output.erase(output.end() - 1);

            std::regex ex("\n");
            output = std::regex_replace(output, ex, "\n./");

            return output;
        } else {
            if (node->name() == name)
                return node->name();
        }
        return "";
    }
};

#endif