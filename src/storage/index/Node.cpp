#include "storage/index/Node.h"
#include "storage/index/Metadata.h"

Node::Node() {
    key = new int[MAX];
    ptr = new Node *[MAX + 1];
    metadata = new Metadata *[MAX]; // Inicializamos el array de punteros a Metadata
    IS_LEAF = false; // Inicializamos IS_LEAF
    size = 0; // Inicializamos size
    for (int i = 0; i < MAX + 1; i++) {
        ptr[i] = nullptr;
    }
    for (int i = 0; i < MAX; i++) {
        metadata[i] = nullptr;
    }
}
