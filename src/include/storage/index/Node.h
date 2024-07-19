
#ifndef NODE_H
#define NODE_H
#include "Metadata.h"
static int MAX = 3;

class Node {
    bool IS_LEAF;
    int *key, size;
    Node **ptr;
    Metadata **metadata;
    friend class BPTree;

public:
    Node();
};

#endif //NODE_H
