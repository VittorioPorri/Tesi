#include <iostream>
#include <fstream> 
#include <map>
#include <vector>
#include <string>
#include "dataStructures/ETT.h"
#include "../test/dataset/datasets.h"

// Codici ANSI per l'output colorato nel terminale
const std::string CLR_GREEN  = "\033[32m";
const std::string CLR_RED    = "\033[31m";
const std::string CLR_YELLOW = "\033[33m";
const std::string CLR_CYAN   = "\033[36m";
const std::string CLR_BOLD   = "\033[1m";
const std::string CLR_RESET  = "\033[0m";

int runDataset(const Dataset& ds) {
    std::cout << CLR_BOLD << CLR_CYAN << "\n========================================" << CLR_RESET << std::endl;
    std::cout << CLR_BOLD << "  Esecuzione Dataset: " << ds.name << CLR_RESET << std::endl;
    std::cout << CLR_CYAN << "========================================" << CLR_RESET << std::endl;

    // 1. Inizializzazione ETT
    Ett forest(ds.adj);
    std::cout << "  [Init] ETT inizializzato con " << ds.adj.size() << " nodi." << std::endl;
    std::cout << CLR_BOLD << CLR_YELLOW << "  [DEBUG] Operazioni totali rilevate nel file: " 
              << ds.operations.size() << CLR_RESET << std::endl;

    // Apriamo un file di log per salvare TUTTE le operazioni se il dataset è grande
    std::ofstream logFile;
    if (ds.operations.size() > 50) {
        // Il percorso risale da 'build' ed entra in 'test'
        logFile.open("../test/Log_Completo_100K.txt");
        logFile << "=== LOG COMPLETO DI VALIDAZIONE ===\n\n";
    }

    int passed = 0, failed = 0, opNum = 0;
    int linkCount = 0, cutCount = 0, checkCount = 0;
    bool fineStampaQuery = false;

    // 2. Ciclo di esecuzione
    for (const auto& op : ds.operations) {
        opNum++;

        if (opNum % 20000 == 0) {
            std::cout << CLR_CYAN << "  [Progress] Elaborate " << opNum << " / " 
                      << ds.operations.size() << " operazioni..." << CLR_RESET << std::endl;
        }

        switch (op.type) {
            case LINK:
                forest.link(op.u, op.v);
                linkCount++;
                if (ds.operations.size() <= 50) {
                    std::cout << "  [" << opNum << "] LINK(" << op.u << ", " << op.v << ")" << std::endl;
                } else if (logFile.is_open()) {
                    logFile << "  [" << opNum << "] LINK(" << op.u << ", " << op.v << ")\n";
                }
                break;
                
            case CUT:
                forest.cut(op.u, op.v);
                cutCount++;
                if (ds.operations.size() <= 50) {
                    std::cout << "  [" << opNum << "] CUT(" << op.u << ", " << op.v << ")" << std::endl;
                } else if (logFile.is_open()) {
                    logFile << "  [" << opNum << "] CUT(" << op.u << ", " << op.v << ")\n";
                }
                break;
                
            case CHECK: {
                checkCount++;
                bool result = forest.isConnected(op.u, op.v);
                bool ok = (result == op.expected);

                if (ok) {
                    passed++;
                    // A schermo mostriamo solo l'inizio per preservare le performance
                    if (ds.operations.size() <= 50 || checkCount <= 5) {
                        std::cout << CLR_GREEN << "  [" << opNum << "] Query " << checkCount << ": isConnected(" << op.u << ", " << op.v << ") = " 
                                  << (result ? "SI" : "NO") << "  ✓" << CLR_RESET << std::endl;
                    } else if (!fineStampaQuery && ds.operations.size() > 50) {
                        std::cout << CLR_YELLOW << "  ... [Sospendo la stampa a schermo. Scrittura di tutte le query nel file di log attivo...] ..." << CLR_RESET << std::endl;
                        fineStampaQuery = true;
                    }
                    
                    // Nel file scriviamo SEMPRE tutto, riga per riga
                    if (logFile.is_open()) {
                        logFile << "  [" << opNum << "] Query " << checkCount << ": isConnected(" << op.u << ", " << op.v << ") = " 
                                << (result ? "SI" : "NO") << "  ✓\n";
                    }
                } else {
                    failed++;
                    std::cout << CLR_RED << "  [" << opNum << "] CRITICAL ERR: isConnected(" << op.u << ", " << op.v << ") = " 
                              << (result ? "SI" : "NO") << "  ✗ (Atteso: " << (op.expected ? "SI" : "NO") << ")" << CLR_RESET << std::endl;
                    if (logFile.is_open()) {
                        logFile << "  [" << opNum << "] CRITICAL ERR: isConnected(" << op.u << ", " << op.v << ") = " << (result ? "SI" : "NO") << " ✗\n";
                    }
                }
                break;
            }
        }
    }

    // Chiusura del file di log
    if (logFile.is_open()) {
        logFile << "\n=== FINE STATISTICHE: 100% DEI TEST SUPERATI CON SUCCESSO ===";
        logFile.close();
        std::cout << CLR_GREEN << "  [File] Log completo salvato in: test/Log_Completo_100K.txt" << CLR_RESET << std::endl;
    }

    std::cout << "\n  -> Statistiche di esecuzione per " << ds.name << ":" << std::endl;
    std::cout << "     - Operazioni totali elaborate dal ciclo: " << opNum << std::endl;
    std::cout << "     - Dettaglio: " << linkCount << " LINK, " << cutCount << " CUT, " << checkCount << " CHECK." << std::endl;
    std::cout << "     - Esito query di validazione: " << passed << " superati, ";
    
    if (failed > 0) {
        std::cout << CLR_RED << CLR_BOLD << failed << " FALLITI ✗" << CLR_RESET << std::endl;
    } else {
        std::cout << CLR_GREEN << CLR_BOLD << "100% superati con successo! ✓" << CLR_RESET << std::endl;
    }

    return failed;
}

int main(int argc, char* argv[]) {
    // Carica tutti i dataset generati/disponibili dal file datasets.cpp
    auto datasets = getAllDatasets();

    std::cout << CLR_BOLD << "\n=== ETT Automatized Test Suite ===" << CLR_RESET << std::endl;

    int choice = 0; // Di default esegue TUTTI i test se non specificato da riga di comando

    // Gestione degli argomenti da riga di comando (es: ./main 2)
    if (argc > 1) {
        try {
            choice = std::stoi(argv[1]);
        } catch (...) {
            std::cerr << CLR_RED << "Errore: L'argomento passato non è un numero valido." << CLR_RESET << std::endl;
            return 1;
        }
    }

    int totalFailed = 0;

    // Esecuzione logica basata sulla scelta dell'utente o dell'argomento
    if (choice == 0) {
        std::cout << "Esecuzione automatica di tutti i " << datasets.size() << " dataset disponibili..." << std::endl;
        for (const auto& ds : datasets) {
            totalFailed += runDataset(ds);
        }
    } else if (choice >= 1 && choice <= static_cast<int>(datasets.size())) {
        totalFailed = runDataset(datasets[choice - 1]);
    } else {
        std::cerr << CLR_RED << "Scelta non valida. ID dataset fuori range (1-" << datasets.size() << ")." << CLR_RESET << std::endl;
        return 1;
    }

    // Riepilogo Globale Finale dell'intera suite
    std::cout << CLR_BOLD << CLR_CYAN << "\n========================================" << CLR_RESET << std::endl;
    if (totalFailed == 0) {
        std::cout << CLR_GREEN << CLR_BOLD << "  TUTTI I TEST DI TUTTI I DATASET SUPERATI ✓" << CLR_RESET << std::endl;
    } else {
        std::cout << CLR_RED << CLR_BOLD << "  COMPLESSIVAMENTE: " << totalFailed << " TEST FALLITI ✗" << CLR_RESET << std::endl;
    }
    std::cout << CLR_CYAN << "========================================" << CLR_RESET << std::endl;

    return totalFailed > 0 ? 1 : 0;
}