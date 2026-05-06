#include "RBTree.h"

/*Dato un nodo {n} esegue una rotazione verso destra su tale nodo
    @param {root} - Puntatore al nodo radice dell'albero
    @param {n} - Puntatore al nodo su cui si esegue la rotazione
*/
void RBTree::rotateRight(Node*& root, Node* n){
    Node* temp= n->left;
    
    if(temp == nullptr){
        return;
    }

    n->left = temp->right;
    if(temp->right != nullptr){
        temp->right->parent = n;
    }

    //Sistemo i puntatori al padre
    temp->parent = n->parent;
    if(temp->parent == nullptr){
        root = temp;
    }else if(n == n->parent->left){
        n->parent->left = temp;
    }else{
        n->parent->right = temp;
    }

    //Collego n e temp
    temp->right = n;
    n->parent = temp;

    n->updateSize();
    temp->updateSize();
}

/*Dato un nodo {n} esegue una rotazione verso sinistra su tale nodo
    @param {root} - Puntatore al nodo radice dell'albero
    @param {n} - Puntatore al nodo su cui si esegue la rotazione
*/
void RBTree::rotateLeft(Node*& root, Node* n){
    Node* temp= n->right;
    
    if(temp == nullptr){
        return;
    }

    n->right = temp->left;
    if(temp->left != nullptr){
        temp->left->parent = n;
    }

    //Sistemo i puntatori al padre
    temp->parent = n->parent;
    if(temp->parent == nullptr){
        root = temp;
    }else if(n == n->parent->left){
        n->parent->left = temp;
    }else{
        n->parent->right = temp;
    }

    //Collego n e temp
    temp->left = n;
    n->parent = temp;

    n->updateSize();
    temp->updateSize();

}

void RBTree::fixInsert(Node*& root, Node* n){


}

void RBTree::fixRemove(Node*& root, Node* n){

    
}

/*Dato un nodo restituisce in output il nodo radice 

    @param {n} - Puntatore al nodo di cui devo cercare la radice 
    @return Puntatore al nodo radice, oppure nullptr se n è nullo 
*/
Node* RBTree::findRoot(Node* n){
    if(n == nullptr){
        return nullptr;
    }

    Node* curr = n;
    while(curr->parent != nullptr){
        curr = curr->parent;
    }   
    return curr;
}

/*Inserisce un Nodo {n} come minimo dell'albero, posizionandolo il piu a sinistra possibile nell'albero

    @param {root} - Puntatore al nodo radice dell'albero
    @param {n} - Puntatore al nodo che deve essere inserito
    @return Puntatore al nodo radice (Nota: potrebbe essere diversa da root a causa della procedura fixInsert
            oppure perche {n} è il primo nodo inserito nel RBTree
*/
Node* RBTree::insertMin(Node* root, Node* n){
    if(root == nullptr){
        return n;
    }

    Node* curr = root;
    while(curr->left != nullptr){
        curr->size++;   //Incremento di uno la size lungo il cammino che porta al padre di n
        curr = curr->left;
    }

    curr->size++;   //Incremento di uno la size del padre di n
    curr->left = n;
    n->parent = curr;

    fixinsert(root, n);
    return root;
}

/*Inserisce un Nodo {n} come massimo dell'albero, posizionandolo il piu a destra possibile nell'albero
    
    @param {root} - Puntatore al nodo radice dell'albero
    @param {n} - Puntatore al nodo che deve essere inserito
    @return Puntatore al nodo radice (Nota potrebbe essere diversa da root a causa della procedura fixInsert 
            oppure perche {n} è il primo nodo inserito nel RBTree)
*/
Node* RBTree::insertMax(Node* root, Node* n){
    if(root == nullptr){
            return n;
        }

    Node* curr = root;
    while(curr->right != nullptr){
        curr->size++;   
        curr = curr->left;
    }

    curr->size++;  
    curr->left = n;
    n->parent = curr;

    fixinsert(root, n);
    return root;
}

Node* RBTree::remove(Node* root, Node* n){

}


Node* RBTree::join(Node* s1, Node* s2){

}


std::pair<Node*, Node*> RBTree::split(Node* root, Node* n){

} 

