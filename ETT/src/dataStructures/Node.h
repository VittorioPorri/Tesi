#ifndef NODE_H
#define NODE_H

enum Color { RED, BLACK };

class Node {
public:
    int rank;
    int size;
    int source, target;
    Node *left, *right, *parent;
    Color color;

    Node(int source, int target);
    void recolor();
    void updateSize();
};

#endif