#ifndef RBTREE_H
#define RBTREE_H

#include <utility>
#include "Node.h"

class RBTree {
private:
    void rotateLeft(Node*& root, Node* n);
    void rotateRight(Node*& root, Node* n);
    void fixInsert(Node*& root, Node* n);
    Node* findSuccessor(Node* n);
    void fixRemove(Node*& root, Node* n, Node* p);
    
public:
    Node* findMin(Node* n);
    Node* findMax(Node* n);
    Node* findRoot(Node* n);
    Node* insertMin(Node* root, Node* n);
    Node* insertMax(Node* root, Node* n);
    Node* remove(Node* root, Node* n);
    Node* detach(Node* root, Node* n);    
    Node* join(Node* s1,Node* i, Node* s2);
    std::pair<Node*, Node*> split(Node* root, Node* n);    
}; 

#endif