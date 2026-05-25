#ifndef DATASETS_H
#define DATASETS_H

#include <map>
#include <vector>
#include <string>

// Tipo di operazione da eseguire sul grafo
enum OpType { LINK, CUT, CHECK };

// Singola operazione di test
struct TestOp {
    OpType type;
    int u, v;
bool expected; // usato solo per CHECK: risultato atteso di isConnected
};

// Dataset completo
struct Dataset {
    std::string name;
    std::map<int, std::vector<int>> adj;
    std::vector<TestOp> operations;
};

// -------------------------------------------------------------------
// DATASET 1 — Piccolo (5 nodi, 2 alberi)
// Albero A: 1 - 2 - 3
// Albero B: 4 - 5
// -------------------------------------------------------------------
inline Dataset datasetSmall() {
    Dataset ds;
ds.name = "Piccolo (5 nodi, 2 alberi)";
    ds.adj = {
        {1, {2}},
        {2, {1, 3}},
        {3, {2}},
        {4, {5}},
        {5, {4}}
    };
    ds.operations = {
        // Connettività iniziale
        {CHECK, 1, 3, true},
        {CHECK, 4, 5, true},
        {CHECK, 1, 4, false},

        // Link 3-4: unisce i due alberi
        {LINK, 3, 4, false},
        {CHECK, 1, 4, true},
        {CHECK, 1, 5, true},

        // Cut 2-3: separa {1,2} da {3,4,5}
        {CUT, 2, 3, false},
        {CHECK, 1, 2, true},
        {CHECK, 1, 3, false},
        {CHECK, 3, 5, true},
    };
    return ds;
}

// -------------------------------------------------------------------
// DATASET 2 — Medio (10 nodi, 3 alberi)
// Albero A: 1 - 2 - 3 - 4
// Albero B: 5 - 6 - 7
// Albero C: 8 - 9 - 10
// -------------------------------------------------------------------
inline Dataset datasetMedium() {
    Dataset ds;
ds.name = "Medio (10 nodi, 3 alberi)";
    ds.adj = {
        {1, {2}},
        {2, {1, 3}},
        {3, {2, 4}},
        {4, {3}},
        {5, {6}},
        {6, {5, 7}},
        {7, {6}},
        {8, {9}},
        {9, {8, 10}},
        {10, {9}}
    };
    ds.operations = {
        // Connettività iniziale: 3 componenti separate
        {CHECK, 1, 4, true},
        {CHECK, 5, 7, true},
        {CHECK, 8, 10, true},
        {CHECK, 1, 5, false},
        {CHECK, 5, 8, false},
        {CHECK, 1, 10, false},

        // Link 4-5: unisce A e B → {1,2,3,4,5,6,7}
        {LINK, 4, 5, false},
        {CHECK, 1, 7, true},
        {CHECK, 2, 6, true},
        {CHECK, 1, 8, false},

        // Link 7-8: unisce tutto → {1..10}
        {LINK, 7, 8, false},
        {CHECK, 1, 10, true},
        {CHECK, 3, 9, true},

        // Cut 4-5: separa {1,2,3,4} da {5,6,7,8,9,10}
        {CUT, 4, 5, false},
        {CHECK, 1, 4, true},
        {CHECK, 5, 10, true},
        {CHECK, 1, 5, false},
        {CHECK, 4, 5, false},

        // Cut 7-8: separa ulteriormente {5,6,7} da {8,9,10}
        {CUT, 7, 8, false},
        {CHECK, 5, 7, true},
        {CHECK, 8, 10, true},
        {CHECK, 5, 8, false},
    };
    return ds;
}

// -------------------------------------------------------------------
// DATASET 3 — Grande (20 nodi, 4 alberi)
// Albero A: catena 1..6
// Albero B: stella centrata in 7 con foglie 8,9,10,11
// Albero C: catena 12..16
// Albero D: catena 17..20
// -------------------------------------------------------------------
inline Dataset datasetLarge() {
    Dataset ds;
ds.name = "Grande (20 nodi, 4 alberi)";
    ds.adj = {
        // Albero A: catena 1-2-3-4-5-6
        {1, {2}},
        {2, {1, 3}},
        {3, {2, 4}},
        {4, {3, 5}},
        {5, {4, 6}},
        {6, {5}},
        // Albero B: stella centrata in 7
        {7, {8, 9, 10, 11}},
        {8, {7}},
        {9, {7}},
        {10, {7}},
        {11, {7}},
        // Albero C: catena 12-13-14-15-16
        {12, {13}},
        {13, {12, 14}},
        {14, {13, 15}},
        {15, {14, 16}},
        {16, {15}},
        // Albero D: catena 17-18-19-20
        {17, {18}},
        {18, {17, 19}},
        {19, {18, 20}},
        {20, {19}}
    };
    ds.operations = {
        // Connettività iniziale
        {CHECK, 1, 6, true},
        {CHECK, 8, 11, true},
        {CHECK, 12, 16, true},
        {CHECK, 17, 20, true},
        {CHECK, 1, 7, false},
        {CHECK, 7, 12, false},
        {CHECK, 12, 17, false},

        // Link 6-7: A+B
        {LINK, 6, 7, false},
        {CHECK, 1, 11, true},
        {CHECK, 3, 9, true},

        // Link 11-12: A+B+C
        {LINK, 11, 12, false},
        {CHECK, 1, 16, true},
        {CHECK, 8, 14, true},

        // Link 16-17: tutto connesso A+B+C+D
        {LINK, 16, 17, false},
        {CHECK, 1, 20, true},
        {CHECK, 5, 18, true},

        // Cut 6-7: separa A da B+C+D
        {CUT, 6, 7, false},
        {CHECK, 1, 6, true},
        {CHECK, 7, 20, true},
        {CHECK, 1, 7, false},
        {CHECK, 6, 8, false},

        // Cut 11-12: separa B da C+D
        {CUT, 11, 12, false},
        {CHECK, 7, 11, true},
        {CHECK, 12, 20, true},
        {CHECK, 11, 12, false},

        // Link 1-17: collega A e D direttamente
        {LINK, 1, 17, false},
        {CHECK, 1, 20, true},
        {CHECK, 6, 19, true},
        {CHECK, 1, 12, true},

        // Cut 3-4: spezza A in {1,2,3} e {4,5,6}
        {CUT, 3, 4, false},
        {CHECK, 1, 3, true},
        {CHECK, 4, 6, true},
        {CHECK, 1, 17, true},  
        {CHECK, 4, 17, false}, 
    };
    return ds;
}

// -------------------------------------------------------------------
// DATASET 4 — Stress (50 nodi, catena + operazioni multiple)
// Un'unica catena 1-2-3-...-50, poi cut e link alternati
// -------------------------------------------------------------------
inline Dataset datasetStress() {
    Dataset ds;
ds.name = "(50 nodi, catena + operazioni intensive)";

    // Catena 1-2-3-...-50
    for (int i = 1; i <= 50; i++) {
        std::vector<int> neighbors;
        if (i > 1) neighbors.push_back(i - 1);
        if (i < 50) neighbors.push_back(i + 1);
        ds.adj[i] = neighbors;
    }

    ds.operations = {
        // Verifica connettività iniziale
        {CHECK, 1, 50, true},
        {CHECK, 1, 25, true},
        {CHECK, 25, 50, true},

        // Cut 25-26: divide in {1..25} e {26..50}
        {CUT, 25, 26, false},
        {CHECK, 1, 25, true},
        {CHECK, 26, 50, true},
        {CHECK, 1, 26, false},
        {CHECK, 25, 26, false},

        // Cut 10-11: divide {1..25} in {1..10} e {11..25}
        {CUT, 10, 11, false},
        {CHECK, 1, 10, true},
        {CHECK, 11, 25, true},
        {CHECK, 10, 11, false},

        // Cut 40-41: divide {26..50} in {26..40} e {41..50}
        {CUT, 40, 41, false},
        {CHECK, 26, 40, true},
        {CHECK, 41, 50, true},
        {CHECK, 40, 41, false},

        // Ora abbiamo 4 componenti: {1..10}, {11..25}, {26..40}, {41..50}
        {CHECK, 5, 15, false},
        {CHECK, 15, 30, false},
        {CHECK, 30, 45, false},

        // Link 10-41: collega {1..10} con {41..50}
        {LINK, 10, 41, false},
        {CHECK, 1, 50, true},
        {CHECK, 5, 45, true},
        {CHECK, 1, 25, false},

        // Link 25-26: ricollega {11..25} con {26..40}
        {LINK, 25, 26, false},
        {CHECK, 11, 40, true},
        {CHECK, 15, 35, true},
        {CHECK, 1, 30, false},

        // Link 1-40: collega tutto
        {LINK, 1, 40, false},
        {CHECK, 1, 50, true},
        {CHECK, 11, 45, true},
        {CHECK, 20, 30, true},

        // Serie di cut per rompere la catena originale
        {CUT, 5, 6, false},
        {CHECK, 1, 5, true},
        {CHECK, 6, 10, true},
        // 6-10 è connesso a 41-50 tramite link(10,41)
        {CHECK, 6, 50, true},
        // 1-5 è connesso a 40 tramite link(1,40), poi a 26-39, poi a 25, poi a 11-24
        {CHECK, 1, 25, true},
    };
    return ds;
}

// -------------------------------------------------------------------
// DATASET 5 — 100K nodi (Catena + 100.000 Operazioni Verificate)
// -------------------------------------------------------------------
inline Dataset dataset100k() {
    Dataset ds;
    const int N = 100000;
    ds.name = "100K nodi (Catena + 100.000 Operazioni Verificate)";

    // Generazione della catena iniziale 1-2-3-...-N
    for (int i = 1; i <= N; i++) {
        std::vector<int> neighbors;
        if (i > 1) neighbors.push_back(i - 1);
        if (i < N) neighbors.push_back(i + 1);
        ds.adj[i] = neighbors;
    }

    // Generiamo esattamente 100.000 operazioni alternate e verificate matematicamente
    // Tagliamo sistematicamente dei segmenti e verifichiamo la connettività
    for (int i = 1; i < N; i += 2) {
        // 1. Tagliamo l'arco tra i e i+1 (es. 1-2, 3-4, 5-6...)
        ds.operations.push_back({CUT, i, i + 1, false});
        
        // 2. Verifichiamo che ora i e i+1 NON siano più connessi (Atteso: false/NO)
        ds.operations.push_back({CHECK, i, i + 1, false});
        
        // 3. Ricolleghiamo l'arco appena tagliato per ripristinare la catena
        ds.operations.push_back({LINK, i, i + 1, false});
        
        // 4. Verifichiamo che ora siano di nuovo connessi (Atteso: true/SI)
        ds.operations.push_back({CHECK, i, i + 1, true});
    }

    // Riempiamo le operazioni rimanenti fino a 100.000 con verifiche globali sulla catena unita
    while (ds.operations.size() < 100000) {
        ds.operations.push_back({CHECK, 1, N, true});
    }

    return ds;
}


// -------------------------------------------------------------------
// Funzione per ottenere un dataset per indice
// -------------------------------------------------------------------
inline std::vector<Dataset> getAllDatasets() {
    return {
        datasetSmall(),
        datasetMedium(),
        datasetLarge(),
        datasetStress(),
        dataset100k()
    };
}

#endif
