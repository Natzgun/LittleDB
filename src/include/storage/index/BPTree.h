
#ifndef BPTREE_H
#define BPTREE_H
#include "Node.h"

class BPTree {
    Node *root;
    void insertInternal(int, Node *, Node *);
    Node *findParent(Node *, Node *);


public:
    BPTree();
    std::pair<std::string, std::string> search(int);
    void insert(int, const std::string &, int);
    void display(Node *);
    Node *getRoot();
    void generateDot(Node *cursor, std::ofstream &file);
    void exportToDot(const std::string &filename);

    void remove(int x);
    void removeInternal(int x, Node *cursor, Node *child);
};


#endif //BPTREE_H
