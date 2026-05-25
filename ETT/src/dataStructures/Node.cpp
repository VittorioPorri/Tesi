#include "Node.h"

/* Implementa il costruttre della classe Node

    @param {source} - Nodo originale da cui parte l'arco
    @param {target} - Nodo originale in cui arriva l'arco
    @return {Node} - Nodo nella struttra dati che rappresenta l'arco  
*/
Node::Node(int source,int target){
    this->source = source;
    this->target = target;
    this->size = 1;
    this->rank = 0;
    this->color = RED;
    this->left = nullptr;
    this->right = nullptr;
    this->parent = nullptr;
    
}

//Ricolora il nodo Rosso di Nero, mentre quello Nero di Rosso
void Node::recolor(){
    if (this->color == RED){
        this->color = BLACK;
    }else{
        this->color = RED;
    }
}

//Aggiorna l'attributo size basandosi sulla size dei sottoalberi
void Node::updateSize(){
    int leftSize = (left != nullptr) ? left->size : 0;
    int rightSize = (right != nullptr) ? right->size : 0;

    this->size = 1 + leftSize + rightSize;
}

//Aggiorna l'attributo rank basandosi sui rank dei sottoalberi
void Node::updateRank(){
    if (left == nullptr && right == nullptr) {
        this->rank = 0;
        return;
    }

    Node* child = (left != nullptr) ? left : right;

    if (child->color == BLACK) {
        this->rank = child->rank + 1;
    } else {
        this->rank = child->rank;
    }
}