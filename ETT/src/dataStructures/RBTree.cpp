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

    temp->left = n;
    n->parent = temp;

    n->updateSize();
    temp->updateSize();

}

/*
* Dato in input un nodo {n} appena inserito se questo viola le proprietà cromatiche 
* (ossia ha un nodo rosso come padre), questa operazione ha il compito di risistemare tali violazioni con
* rotazioni e ricolorazioni opportune
*/
void RBTree::fixInsert(Node*& root, Node* n){
    while(n != root  && n->parent->color == RED){
        Node* p = n->parent;
        Node* g = p->parent;

        if(p == g->left){
            Node* u = g->right;
            // CASO 2:
            if(u != nullptr && u->color == RED){
                p->recolor();
                u->recolor();
                g->recolor();

                n = g;
            }else{
                // CASO 3:
                if(n == p->right){
                    n = p;
                    rotateLeft(root, n);
                    p = n->parent;
                }
                // CASO 4:
                p->recolor();
                g->recolor();
                rotateRight(root, g);
            }

        }else{
            Node* u = g->left;
            // CASO 2:
            if(u != nullptr && u->color == RED){
                p->recolor();
                u->recolor();
                g->recolor();

                n = g;
            }else{
                // CASO 3:
                if(n == p->left){
                    n = p;
                    rotateRight(root, n);
                    p = n->parent;
                }
                // CASO 4:
                p->recolor();
                g->recolor();
                rotateLeft(root, g);
            }
        }
    }
    // CASO 1
    root->color = BLACK;
}

/*Dato un nodo restituisce in output il nodo minimo, ossia nodo il più a sinistra possibile nell'albero  

    @param {n} - Puntatore al nodo da cui devo cercare il minimo  
    @return Puntatore al nodo minimo, oppure nullptr se n è nullo 
*/
Node* RBTree::findMin(Node* n){
    if(n == nullptr){
        return nullptr;
    }

    Node* temp = n;
    while(temp->left != nullptr){
        temp = temp->left;
    }

    return temp;
}

/*Dato un nodo, restituisce il suo successore nell'ordine delle chiavi

    @param {n} - Puntatore al nodo da cui devo cercare il minimo  
    @return Puntatore al nodo minimo, oppure nullptr se n è nullo 
*/
Node* RBTree::findSuccessor(Node* n){
    if(n == nullptr){
        return nullptr;
    }

    if(n->right != nullptr){
        return findMin(n->right);
    }else{
        Node* u = n;
        while(u->parent != nullptr && u->parent->right == u){
            u = u->parent;
        }
        return u->parent;
    }

}

/*
* Dato in input un nodo {n} da rimuovere se questo e nero la sua rimozioe violerebbe 
* la proprietà della black-height, questa operazione ha il compito di risistemare 
* tale violazione con rotazioni e ricolorazioni opportune in modo tale da permettere la corretta
* rimozione del nodo.
*/
void RBTree::fixRemove(Node*& root, Node* n){
    while(n != root && n->color == BLACK){
        Node* p = n->parent;
        Node* f;
        
        
        if (n == p->left){
            f = p->right;

            //CASO 1:
            if(f != nullptr && f->color == RED){
                f->recolor();
                p->recolor();
                rotateLeft(root, p);
                f = p->right;
            }
            //CASO 2:
            if((f->left  == nullptr || f->left->color == BLACK) && (f->right == nullptr || f->right->color == BLACK)){
                f->recolor();
                if(p->color == BLACK){
                    n = p;
                }else{
                    p->recolor();
                    n = root;
                }
            }else{
                //CASO 3:
                if(f->right == nullptr || f->right->color == BLACK){
                    if(f->left){
                        f->left->recolor();
                    }
                    f->recolor();
                    rotateRight(root, f);
                    f = p->right; 
                }
                //CASO 4:
                f->color = p->color;
                p->color = BLACK;
                if (f->right) {
                    f->right->recolor();
                }
                rotateLeft(root, p);
                n = root;
            }

        }else{
            f = p->left;

            //CASO 1:
            if(f != nullptr && f->color == RED){
                f->recolor();
                p->recolor();
                rotateRight(root, p);
                f = p->left;
            }

            //CASO 2:
            if((f->left  == nullptr || f->left->color == BLACK) && (f->right == nullptr || f->right->color == BLACK)){
                f->recolor();
                if(p->color == BLACK){
                    n = p;
                }else{
                    p->recolor();
                    n = root;
                }
            }else{
                //CASO 3:
                if(f->left == nullptr || f->left->color == BLACK){
                    if(f->right){
                        f->right->recolor();
                    }
                    f->recolor();
                    rotateLeft(root, f);
                    f = p->left; 
                }
                //CASO 4:
                f->color = p->color;
                p->color = BLACK;
                if (f->left) {
                    f->left->recolor();
                }
                rotateRight(root, p);
                n = root;
            }
        }
    }
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

/*Inserisce un Nodo {n} come minimo dell'albero, posizionandolo il più a sinistra possibile nell'albero

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
        curr->size++;   
        curr = curr->left;
    }

    curr->size++;   //Incremento di uno la size del padre di n
    curr->left = n;
    n->parent = curr;

    fixInsert(root, n);

    return root;
}

/*Inserisce un Nodo {n} come massimo dell'albero, posizionandolo il più a destra possibile nell'albero
    
    @param {root} - Puntatore al nodo radice dell'albero
    @param {n} - Puntatore al nodo che deve essere inserito
    @return Puntatore al nodo radice (Nota: potrebbe essere diversa da root a causa della procedura fixInsert 
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

    fixInsert(root, n);
    
    return root;
}

/*Rimuove un nodo {n} dall'albero

    @param {root} - Puntatore al nodo radice dell'albero
    @param {n} - Puntatore al nodo che deve essere rimosso
    @return Puntatore al nodo radice (Nota: potrebbe essere diversa da root a causa della procedura fixRemove 
            oppure potrebbe essere nullptr perche {n} è l'unico nodo nel RBTree)
*/
Node* RBTree::remove(Node* root, Node* n){
    if(n == nullptr){
        return root;
    }

    if(n->right != nullptr && n->left != nullptr){
        Node* s = findSuccessor(n);
        n->source = s->source;
        n->target = s->target;
        n = s;
    }


    fixRemove(root, n);
    
    Node* p = n->parent;
    Node* child = (n->left != nullptr) ? n->left : n->right;

    if (n->left == nullptr && n->right == nullptr) {
        if (p == nullptr) {
            root = nullptr; 
        } else {
            if (p->left == n) {
                p->left = nullptr;
            }else{ 
                p->right = nullptr;
            }
        }
    } else {
        if (p == nullptr) {
            root = child;
        } else {
            if (p->left == n) {
                p->left = child;
            }else{ 
                p->right = child;
            }
        }
        if (child != nullptr) {
            child->parent = p;
        }
    }

    delete n; 
    return root;
}


Node* RBTree::join(Node* s1, Node* s2){

}


std::pair<Node*, Node*> RBTree::split(Node* root, Node* n){

} 

