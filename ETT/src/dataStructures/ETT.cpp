#include "ETT.h"
#include <stack>

/* Funzione helper iterativa per la costruzione del tour di una componente connessa.
 * Sostituisce la versione ricorsiva per evitare stack overflow su grafi molto grandi (es. 100k nodi).
 * Implementa la stessa logica DFS post-order della versione ricorsiva generando la sequenza:
 *   (u,u) -> (u,v) -> [Tour v] -> (v,u)
 * tramite uno stack esplicito invece della pila di chiamate del sistema operativo.
 *
 * Per ogni nodo u, il frame mantiene:
 *   - u, parent     : nodo corrente e suo genitore
 *   - childIndex    : prossimo figlio da esaminare in adj[u]
 *   - tour          : tour parziale accumulato per u
 *   - pendingUV     : arco (u->v) allocato prima di scendere nel figlio v,
 *                     usato per il join al ritorno da v
 *
 * @param start   - ID del vertice di partenza della componente.
 * @param adj     - Riferimento alla lista di adiacenza del grafo.
 * @param visited - Set dei nodi già processati per gestire foreste disconnesse.
 */
void Ett::buildTourDFS(int start,
                       const std::map<int, std::vector<int>>& adj,
                       std::unordered_set<int>& visited)
{
    struct Frame {
        int u;
        int parent;
        size_t childIndex;
        Node* tour;
        Node* pendingUV;  // arco verso il figlio in cui siamo scesi (nullptr se nessuno)
    };

    std::stack<Frame> stk;

    // --- Inizializza il frame radice ---
    visited.insert(start);
    Node* startNode = new Node(start, start);
    startNode->color = BLACK;
    startNode->updateSize();
    mapNode[start] = startNode;

    stk.push({start, -1, 0, startNode, nullptr});

    while (!stk.empty()) {
        Frame& f = stk.top();
        int u = f.u;

        const std::vector<int>* neighbors = nullptr;
        if (adj.count(u)) {
            neighbors = &adj.at(u);
        }

        // Cerca il prossimo figlio non visitato
        bool foundChild = false;
        if (neighbors != nullptr) {
            while (f.childIndex < neighbors->size()) {
                int v = (*neighbors)[f.childIndex];
                f.childIndex++;

                if (v == f.parent || visited.count(v)) {
                    continue;
                }

                // Trovato figlio v: crea l'arco (u->v) e memorizzalo come pendingUV
                Node* uv = new Node(u, v);
                mapEdge[{u, v}] = uv;
                f.pendingUV = uv;

                // Crea il nodo ETT per v e spingi un nuovo frame
                visited.insert(v);
                Node* vNode = new Node(v, v);
                vNode->color = BLACK;
                vNode->updateSize();
                mapNode[v] = vNode;

                stk.push({v, u, 0, vNode, nullptr});
                foundChild = true;
                break;
            }
        }

        if (!foundChild) {
            // Tutti i figli di u sono stati processati: torna al padre
            Node* completedTour = f.tour;
            stk.pop();

            if (!stk.empty()) {
                Frame& par = stk.top();
                Node* uv = par.pendingUV;   // arco (par.u -> u) emesso prima di scendere in u
                par.pendingUV = nullptr;

                // Arco di ritorno (u -> par.u)
                Node* vu = new Node(u, par.u);
                mapEdge[{u, par.u}] = vu;

                // Equivalente ricorsivo originale:
                //   currentTour = join(currentTour, uv, subtreeTour)   // entra nel sottoalbero
                //   currentTour = join(currentTour, vu, nullptr)        // arco di ritorno
                par.tour = rbEngine.join(par.tour, uv, completedTour);
                par.tour = rbEngine.join(par.tour, vu, nullptr);
            }
            // Se lo stack è vuoto siamo tornati alla radice: completedTour era già in mapNode[start]
        }
    }
}

/* Inizializza la foresta di Euler Tour Trees a partire da una struttura a grafo.
 * @param adj - Mappa di adiacenza che rappresenta la foresta originale.
 */
Ett::Ett(const std::map<int, std::vector<int>>& adj) {
    std::unordered_set<int> visited;

    for (auto const& [u, neighbors] : adj) {
        if (visited.find(u) == visited.end()) {
            buildTourDFS(u, adj, visited);
        }
    }
}

/* Dato in input un nodo {v} restituisce in output l'ETT che ha come nuovo start il nodo {v}
*   @param {v} - Puntatore al nodo
*   @return La radice dell'albero che rappresenta il nuovo ETT che comincia da {v}
*/
Node* Ett::reroot(int v) {
    if (mapNode.find(v) == mapNode.end()) return nullptr;

    Node* e_start = mapNode[v]; 
    Node* root = rbEngine.findRoot(e_start);
    auto [TL, TR] = rbEngine.split(root, e_start);

    if (TL == nullptr){
        // e_start è già il primo nel tour, basta reinserirlo come minimo di TR
        return rbEngine.insertMin(TR, e_start); 
    }

    Node* p_old = rbEngine.findMin(TL);
    int source = p_old->source; 
    int target = p_old->target;

    mapEdge.erase({p_old->source, p_old->target});
    TL = rbEngine.detach(TL, p_old); 

    // Reimposto p_old come nuovo nodo perno con gli stessi dati
    p_old->source = source;
    p_old->target = target;
    mapEdge[{source, target}] = p_old;
    
    Node* merged = rbEngine.join(TR, p_old, TL);
    merged = rbEngine.insertMin(merged, e_start);
    return merged;
}

/* Dati in input due nodi {u} e {v} restituisce true se entrambi i nodi appartengono allo 
* stesso RB-Tree altrimenti restituisce false
*   @param {u} - Puntatore al primo nodo
*   @param {v} - Puntatore al secondo nodo
*   @return Restituisce {True} se i nodi sono nello stesso RB-Tree o sono lo stesso nodo, 
*           altrimenti {False} 
*/
bool Ett::isConnected(int u, int v) {
    if (mapNode.find(u) == mapNode.end() || mapNode.find(v) == mapNode.end()){
        return u == v;
    }

    Node* rootU = rbEngine.findRoot(mapNode[u]);
    Node* rootV = rbEngine.findRoot(mapNode[v]);

    return rootU == rootV;
}

/* Dati in input due nodi {u} e {v} crea un arco che li collega  
*   @param {u} - Puntatore al primo nodo
*   @param {v} - Puntatore al secondo nodo 
*/
void Ett::link(int u, int v) {
    if (isConnected(u, v)){
        return; 
    }

    reroot(u);
    reroot(v);

    Node* rootU = rbEngine.findRoot(mapNode[u]);
    Node* rootV = rbEngine.findRoot(mapNode[v]);

    Node* uv = new Node(u, v);
    Node* vu = new Node(v, u);
    mapEdge[{u, v}] = uv;
    mapEdge[{v, u}] = vu;

    Node* tempRoot = rbEngine.join(rootU, uv, rootV);
    rbEngine.insertMax(tempRoot, vu);    
}

/* Dati in input due nodi {u} e {v} elimina l'arco che li collega  
*   @param {u} - Puntatore al primo nodo
*   @param {v} - Puntatore al secondo nodo 
*/
void Ett::cut(int u, int v) {
    if (mapEdge.find({u, v}) == mapEdge.end()){
        return;
    } 

    Node* uv = mapEdge[{u, v}];
    Node* vu = mapEdge[{v, u}];

    Node* root = rbEngine.findRoot(uv);
    auto [TL, TR] = rbEngine.split(root, uv);

    Node* rootOfvu = rbEngine.findRoot(vu);
    Node* rootOfTR = (TR != nullptr) ? rbEngine.findRoot(TR) : nullptr;
    
    if (rootOfvu == rootOfTR && rootOfTR != nullptr) {
        auto [T_middle, T_right] = rbEngine.split(TR, vu);
        
        if (T_right != nullptr) {
            // Estraggo min(T_right) come perno per join(TL, perno, T_right\perno)
            Node* p_old = rbEngine.findMin(T_right);
            int source = p_old->source; 
            int target = p_old->target;

            mapEdge.erase({p_old->source, p_old->target});
            T_right = rbEngine.detach(T_right, p_old); 

            // Reimposto p_old come nuovo nodo perno con gli stessi dati
            p_old->source = source;
            p_old->target = target;
            mapEdge[{source, target}] = p_old;

            rbEngine.join(TL, p_old, T_right);
        }
    } else {
        auto [T_left, T_middle] = rbEngine.split(TL, vu);

        if (TR != nullptr) {
            // Estraggo min(TR) come perno per join(T_left, perno, TR\perno)
            Node* p_old = rbEngine.findMin(TR);
            int source = p_old->source; 
            int target = p_old->target;

            mapEdge.erase({p_old->source, p_old->target});
            TR = rbEngine.detach(TR, p_old); 

            // Reimposto p_old come nuovo nodo perno con gli stessi dati
            p_old->source = source;
            p_old->target = target;
            mapEdge[{source, target}] = p_old;
            rbEngine.join(T_left, p_old, TR);
        }
    }

    mapEdge.erase({u, v});
    mapEdge.erase({v, u});

    delete uv;
    delete vu;
}