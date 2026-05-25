#ifndef ETT_H
#define ETT_H

#include <unordered_map>
#include <utility>
#include <map>      
#include <vector>  
#include <unordered_set> 
#include "RBTree.h"
#include "Node.h"

// Funzione di hash custom per usare std::pair come chiave in unordered_map.
struct PairHash {
    inline size_t operator()(const std::pair<int, int> & v) const {
        return std::hash<int>{}(v.first) ^ (std::hash<int>{}(v.second) << 1);
    }
};

class Ett {
private:
    RBTree rbEngine;
    std::unordered_map<std::pair<int, int>, Node*, PairHash> mapEdge;
    std::unordered_map<int, Node*> mapNode;

    void buildTourDFS(int start, const std::map<int, std::vector<int>>& adj, std::unordered_set<int>& visited);
    Node* reroot(int v);

public:
    Ett(const std::map<int, std::vector<int>>& adj);
    bool isConnected(int u, int v);
    void link(int u, int v);
    void cut(int u, int v);

};

#endif