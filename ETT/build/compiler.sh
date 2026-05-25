#!/bin/bash
cd "$(dirname "$0")/.."
g++ -O2 -o build/main src/main.cpp src/dataStructures/Node.cpp src/dataStructures/RBTree.cpp src/dataStructures/ETT.cpp -std=c++17
