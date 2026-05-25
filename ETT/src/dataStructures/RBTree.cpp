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
    n->updateRank();
    temp->updateSize();
    temp->updateRank();
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
    n->updateRank();
    temp->updateSize();
    temp->updateRank();
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
        if(g == nullptr) break;

        if(p == g->left){
            Node* u = g->right;
            // CASO 2:
            if(u != nullptr && u->color == RED){
                p->recolor();
                u->recolor();
                g->recolor();

                p->updateRank();
                u->updateRank();
                g->updateRank();

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

                p->updateRank();
                u->updateRank();
                g->updateRank();

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

/*Dato un nodo restituisce in output il nodo massimo, ossia nodo il più a destra possibile nell'albero  

    @param {n} - Puntatore al nodo da cui devo cercare il massimo  
    @return Puntatore al nodo massimo, oppure nullptr se n è nullo 
*/
Node* RBTree::findMax(Node* n){
    if(n == nullptr){
        return nullptr;
    }

    Node* temp = n;
    while(temp->right != nullptr){
        temp = temp->right;
    }

    return temp;
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
void RBTree::fixRemove(Node*& root, Node* n, Node* p){
    while(n != root && (n == nullptr || n->color == BLACK)){

        if (p == nullptr) {
            break;
        }

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
            if(f == nullptr || ((f->left  == nullptr || f->left->color == BLACK) && (f->right == nullptr || f->right->color == BLACK))){
                if(f){
                    f->recolor();
                    f->updateRank();
                }

                if(p->color == BLACK){
                    n = p;
                    p = n->parent;
                }else{
                    p->recolor();
                    p->updateRank();
                    n = root;
                }
            }else{
                //CASO 3:
                if(f->right == nullptr || f->right->color == BLACK){
                    if(f->left){
                        f->left->recolor();
                        f->left->updateRank();
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
                    f->right->updateRank();
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
            if(f == nullptr || ((f->left  == nullptr || f->left->color == BLACK) && (f->right == nullptr || f->right->color == BLACK))){
                if(f){
                    f->recolor();
                    f->updateRank();
                }

                if(p->color == BLACK){
                    n = p;
                    p = n->parent;
                }else{
                    p->recolor();
                    p->updateRank();
                    n = root;
                }
            }else{
                //CASO 3:
                if(f->left == nullptr || f->left->color == BLACK){
                    if(f->right){
                        f->right->recolor();
                        f->right->updateRank();
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
                    f->left->updateRank();
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
        curr = curr->right;
    }

    curr->size++;  
    curr->right = n;
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

    Node* temp = p;
    while (temp != nullptr){
        temp->updateRank();
        temp->updateSize();
        temp = temp->parent;
    }

    
    if(n->color == BLACK){
        fixRemove(root, child, p);
    }

    delete n;

    if (root != nullptr) {
        root->color = BLACK;
        root->updateRank();
    }
    
    return root;
}

/*Rimuove un nodo {n} dall'albero SENZA deallocarlo (a differenza di remove).
  Il nodo viene isolato (left=right=parent=nullptr) e restituito pronto per il riuso.

    @param {root} - Puntatore al nodo radice dell'albero
    @param {n} - Puntatore al nodo che deve essere staccato
    @return Puntatore al nodo radice aggiornato
*/
Node* RBTree::detach(Node* root, Node* n){
    if(n == nullptr){
        return root;
    }

    if(n->right != nullptr && n->left != nullptr){
        Node* s = findSuccessor(n);
        n->source = s->source;
        n->target = s->target;
        n = s;
    }

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

    Node* temp = p;
    while (temp != nullptr){
        temp->updateRank();
        temp->updateSize();
        temp = temp->parent;
    }

    if(n->color == BLACK){
        fixRemove(root, child, p);
    }

    // Isolo il nodo senza deallocarlo
    n->left = nullptr;
    n->right = nullptr;
    n->parent = nullptr;
    n->size = 1;
    n->rank = 0;
    n->color = RED;

    if (root != nullptr) {
        root->color = BLACK;
        root->updateRank();
    }
    
    return root;
}

/*Presi due Alberi {s1} e {s2} e un nodo {i} l'obbiettivo dell'operazione è fondere i due 
  alberi in un unico albero utilizzando il nodo {i} come perno della fusione 
    
    @param {s1} - Puntatore al nodo radice dell'albero s1
    @param {s2} - Puntatore al nodo radice dell'albero s2
    @param {i} - Puntatore al nodo perno
    @return Puntatore al nodo radice (Nota: potrebbe essere diversa da una delle due radici
            a causa della procedura fixInsert)
*/
Node* RBTree::join(Node* s1, Node* i, Node* s2) {
    // Caso speciale: concatenazione senza nodo perno
    if (i == nullptr) {
        if (s1 == nullptr) return s2;
        if (s2 == nullptr) return s1;
        // Estraggo il minimo di s2 e lo uso come perno
        Node* pivot = findMin(s2);
        s2 = detach(s2, pivot);
        return join(s1, pivot, s2);
    }

    if (s1 == nullptr){
        return insertMin(s2, i); 
    }    

    if (s2 == nullptr){
        return insertMax(s1, i);
    }  

    if (s1->rank >= s2->rank) {
        Node* curr = s1;

        curr->color = BLACK;
        s2->color = BLACK;

        while (curr->rank > s2->rank) {
            curr = curr->right;
        }

        Node* p = curr->parent;
        
        i->left = curr;
        i->right = s2;
        i->parent = p;
        i->color = RED; 

        curr->parent = i;
        s2->parent = i;

        if (p == nullptr) {
            i->color = BLACK;
            i->updateSize();
            i->updateRank();
            return i;
        } else {
            p->right = i;
            
            Node* temp = i;
            while (temp != nullptr) {
                temp->updateSize();
                temp->updateRank();
                temp = temp->parent;
            }

            fixInsert(s1, i);

            return findRoot(s1);
        }
    } else {
        Node* curr = s2;

        curr->color = BLACK;
        s1->color = BLACK;

        while (curr->rank > s1->rank) {
            curr = curr->left;
        }

        Node* p = curr->parent;
        
        i->left = s1;
        i->right = curr;
        i->parent = p;
        i->color = RED; 

        curr->parent = i;
        s1->parent = i;

        if (p == nullptr) {
            i->color = BLACK;
            i->updateSize();
            i->updateRank();
            return i;
        } else {
            p->left = i;
            
            Node* temp = i;
            while (temp != nullptr) {
                temp->updateSize();
                temp->updateRank();
                temp = temp->parent;
            }

            fixInsert(s2, i);

            return findRoot(s2);
        }
    }
}


std::pair<Node*, Node*> RBTree::split(Node* root, Node* n){
    Node* rootTl = n->left;
    if(n->left){
        rootTl->parent = nullptr;
    }

    Node* rootTr = n->right;
    if(n->right){
        rootTr->parent = nullptr;
    }
    
    Node* curr = n->parent; 
    Node* last = n;

    //Isolo n
    n->left = nullptr;
    n->right = nullptr;
    n->parent = nullptr;
    n->size = 1;
    n->rank = 0;
    n->color = RED;

    while(curr != nullptr){
        Node* p = curr->parent;

        // Determino da che lato veniva last PRIMA di modificare i puntatori
        bool lastWasLeft = (curr->left == last);

        // Taglio il link tra curr e last (già processato)
        if(lastWasLeft){
            curr->left = nullptr;
        }else{
            curr->right = nullptr;
        }

        // L'altro sotto-albero è quello rimasto
        Node* otherSub = lastWasLeft ? curr->right : curr->left;
        
        if(otherSub){
            otherSub->parent = nullptr;
        }

        //Isolo curr
        curr->left = nullptr;
        curr->right = nullptr;
        curr->parent = nullptr;
        curr->size = 1;
        curr->rank = 0;
        curr->color = RED;

        if(lastWasLeft){
            // last veniva da sinistra → otherSub è il sottoalbero destro
            // curr e otherSub vanno aggiunti a rootTr
            rootTr = join(rootTr, curr, otherSub); 
        }else{
            // last veniva da destra → otherSub è il sottoalbero sinistro
            // otherSub e curr vanno aggiunti a rootTl
            rootTl = join(otherSub, curr, rootTl);
        }

        last = curr;
        curr = p;
    }

    return {rootTl, rootTr};
} 

