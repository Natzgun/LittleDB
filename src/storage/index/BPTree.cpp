#include "storage/index/BPTree.h"

#include <ostream>
#include <string>
#include <fstream>

#include "storage/index/Metadata.h"
#include "storage/index/Node.h"
#include <iostream>

using namespace std;

BPTree::BPTree() {
    root = nullptr;
}

// Search operation
pair<string,string> BPTree::search(int x) {
    if (root == nullptr) {
        return {"Null_Index", "Tree is empty"};
    } else {
        Node *cursor = root;
        while (!cursor->IS_LEAF) {
            int i;
            for (i = 0; i < cursor->size; i++) {
                if (x < cursor->key[i]) {
                    cursor = cursor->ptr[i];
                    break;
                }
                if (i == cursor->size - 1) {
                    cursor = cursor->ptr[i + 1];
                    break;
                }
            }
        }
        for (int i = 0; i < cursor->size; i++) {
            if (cursor->key[i] == x) {
                pair<string, string> block;
                block.first = to_string(x);
                block.second = cursor->metadata[i]->block + "#" + to_string(cursor->metadata[i]->offset);
                return block;
            }
        }
        return {"Null_Index", "Not found"};
    }
}

// Insert Operation
void BPTree::insert(int x, const std::string &block, int offset) {
    Metadata *newMetadata = new Metadata{block, offset}; // Creamos la metadata

    if (root == nullptr) {
        root = new Node;
        root->key[0] = x;
        root->IS_LEAF = true;
        root->size = 1;
        root->metadata[0] = newMetadata; // Asignamos la metadata
    } else {
        Node *cursor = root;
        Node *parent = nullptr;
        while (!cursor->IS_LEAF) {
            parent = cursor;
            int i;
            for (i = 0; i < cursor->size; i++) {
                if (x < cursor->key[i]) {
                    cursor = cursor->ptr[i];
                    break;
                }
                if (i == cursor->size - 1) {
                    cursor = cursor->ptr[i + 1];
                    break;
                }
            }
        }
        if (cursor->size < MAX) {
            int i = 0;
            while (i < cursor->size && x > cursor->key[i])
                i++;
            for (int j = cursor->size; j > i; j--) {
                cursor->key[j] = cursor->key[j - 1];
                cursor->metadata[j] = cursor->metadata[j - 1]; // Movemos las metadata también
            }
            cursor->key[i] = x;
            cursor->metadata[i] = newMetadata; // Asignamos la nueva metadata
            cursor->size++;
            cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
            cursor->ptr[cursor->size - 1] = nullptr;
        } else {
            Node *newLeaf = new Node;
            int virtualNode[MAX + 1];
            Metadata *virtualMetadata[MAX + 1]; // Array virtual para metadata
            for (int i = 0; i < MAX; i++) {
                virtualNode[i] = cursor->key[i];
                virtualMetadata[i] = cursor->metadata[i];
            }
            int i = 0;
            while (i < MAX && x > virtualNode[i])
                i++;
            for (int j = MAX; j > i; j--) {
                virtualNode[j] = virtualNode[j - 1];
                virtualMetadata[j] = virtualMetadata[j - 1]; // Movemos las metadata también
            }
            virtualNode[i] = x;
            virtualMetadata[i] = newMetadata; // Asignamos la nueva metadata
            newLeaf->IS_LEAF = true;
            cursor->size = (MAX + 1) / 2;
            newLeaf->size = MAX + 1 - cursor->size;
            cursor->ptr[cursor->size] = newLeaf;
            newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX];
            cursor->ptr[MAX] = nullptr;
            for (i = 0; i < cursor->size; i++) {
                cursor->key[i] = virtualNode[i];
                cursor->metadata[i] = virtualMetadata[i]; // Asignamos las metadata
            }
            for (i = 0; i < newLeaf->size; i++) {
                newLeaf->key[i] = virtualNode[cursor->size + i];
                newLeaf->metadata[i] = virtualMetadata[cursor->size + i]; // Asignamos las metadata
            }
            if (cursor == root) {
                Node *newRoot = new Node;
                newRoot->key[0] = newLeaf->key[0];
                newRoot->ptr[0] = cursor;
                newRoot->ptr[1] = newLeaf;
                newRoot->IS_LEAF = false;
                newRoot->size = 1;
                root = newRoot;
            } else {
                insertInternal(newLeaf->key[0], parent, newLeaf);
            }
        }
    }
}

// Insert Operation
void BPTree::insertInternal(int x, Node *cursor, Node *child) {
    if (cursor->size < MAX) {
        int i = 0;
        while (i < cursor->size && x > cursor->key[i])
            i++;
        for (int j = cursor->size; j > i; j--) {
            cursor->key[j] = cursor->key[j - 1];
        }
        for (int j = cursor->size + 1; j > i + 1; j--) {
            cursor->ptr[j] = cursor->ptr[j - 1];
        }
        cursor->key[i] = x;
        cursor->ptr[i + 1] = child;
        cursor->size++;
    } else {
        Node *newInternal = new Node;
        int virtualKey[MAX + 1];
        Node *virtualPtr[MAX + 2];
        for (int i = 0; i < MAX; i++) {
            virtualKey[i] = cursor->key[i];
        }
        for (int i = 0; i < MAX + 1; i++) {
            virtualPtr[i] = cursor->ptr[i];
        }
        int i = 0;
        while (i < MAX && x > virtualKey[i])
            i++;
        for (int j = MAX; j > i; j--) {
            virtualKey[j] = virtualKey[j - 1];
        }
        virtualKey[i] = x;
        for (int j = MAX + 1; j > i + 1; j--) {
            virtualPtr[j] = virtualPtr[j - 1];
        }
        virtualPtr[i + 1] = child;
        newInternal->IS_LEAF = false;
        cursor->size = (MAX + 1) / 2;
        newInternal->size = MAX - cursor->size;
        for (i = 0; i < cursor->size; i++) {
            cursor->key[i] = virtualKey[i];
        }
        for (i = 0; i < cursor->size + 1; i++) {
            cursor->ptr[i] = virtualPtr[i];
        }
        for (i = 0; i < newInternal->size; i++) {
            newInternal->key[i] = virtualKey[cursor->size + i];
        }
        for (i = 0; i < newInternal->size + 1; i++) {
            newInternal->ptr[i] = virtualPtr[cursor->size + i + 1];
        }
        if (cursor == root) {
            Node *newRoot = new Node;
            newRoot->key[0] = cursor->key[cursor->size];
            newRoot->ptr[0] = cursor;
            newRoot->ptr[1] = newInternal;
            newRoot->IS_LEAF = false;
            newRoot->size = 1;
            root = newRoot;
        } else {
            insertInternal(cursor->key[cursor->size], findParent(root, cursor), newInternal);
        }
    }
}

// Find the parent
Node *BPTree::findParent(Node *cursor, Node *child) {
    if (cursor->IS_LEAF || (cursor->ptr[0] && cursor->ptr[0]->IS_LEAF)) {
        return nullptr;
    }
    for (int i = 0; i < cursor->size + 1; i++) {
        if (cursor->ptr[i] == child) {
            return cursor;
        } else {
            Node *parent = findParent(cursor->ptr[i], child);
            if (parent != nullptr)
                return parent;
        }
    }
    return nullptr;
}

// Print the tree
void BPTree::display(Node *cursor) {
    if (cursor != nullptr) {
        for (int i = 0; i < cursor->size; i++) {
            cout << cursor->key[i] << " ";
        }
        cout << "\n";
        if (!cursor->IS_LEAF) {
            for (int i = 0; i < cursor->size + 1; i++) {
                display(cursor->ptr[i]);
            }
        }
    }
}

// Get the root
Node *BPTree::getRoot() {
    return root;
}

void BPTree::generateDot(Node *cursor, ofstream &file) {
    if (cursor == nullptr) return;

    // Si es una hoja
    if (cursor->IS_LEAF) {
        file << "  node" << cursor << " [label=\"";
        for (int i = 0; i < cursor->size; i++) {
            file << cursor->key[i] << "\\n";
        }
        file << "\", shape=box];\n";
    } else {
        file << "  node" << cursor << " [label=\"";
        for (int i = 0; i < cursor->size; i++) {
            file << cursor->key[i] << "\\n";
        }
        file << "\", shape=ellipse];\n";

        for (int i = 0; i < cursor->size + 1; i++) {
            generateDot(cursor->ptr[i], file);
            if (cursor->ptr[i]) {
                file << "  node" << cursor << " -> node" << cursor->ptr[i] << ";\n";
            }
        }
    }
}

void BPTree::exportToDot(const string &filename) {
    ofstream file(filename);
    file << "digraph BPTree {\n";
    generateDot(root, file);
    file << "}\n";
    file.close();
}

// Delete Operation
void BPTree::remove(int x) {
    if (root == nullptr) {
        cout << "Tree is empty\n";
        return;
    } else {
        Node *cursor = root;
        Node *parent = nullptr;
        int leftSibling, rightSibling;

        while (!cursor->IS_LEAF) {
            parent = cursor;
            int i;
            for (i = 0; i < cursor->size; i++) {
                leftSibling = i - 1;
                rightSibling = i + 1;
                if (x < cursor->key[i]) {
                    cursor = cursor->ptr[i];
                    break;
                }
                if (i == cursor->size - 1) {
                    leftSibling = i;
                    rightSibling = i + 2;
                    cursor = cursor->ptr[i + 1];
                    break;
                }
            }
        }

        bool found = false;
        int pos;
        for (pos = 0; pos < cursor->size; pos++) {
            if (cursor->key[pos] == x) {
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "Key " << x << " is not found\n";
            return;
        }

        for (int i = pos; i < cursor->size - 1; i++) {
            cursor->key[i] = cursor->key[i + 1];
            cursor->metadata[i] = cursor->metadata[i + 1]; // Mueve las metadata también
        }
        cursor->size--;

        if (cursor == root) {
            if (cursor->size == 0) {
                delete[] cursor->key;
                delete[] cursor->ptr;
                delete[] cursor->metadata; // Liberamos la memoria de metadata
                delete cursor;
                root = nullptr;
            }
            return;
        }

        cursor->ptr[cursor->size] = cursor->ptr[cursor->size + 1];
        cursor->ptr[cursor->size + 1] = nullptr;

        if (cursor->size >= (MAX + 1) / 2) {
            return;
        }

        if (rightSibling <= parent->size) {
            Node *rightNode = parent->ptr[rightSibling];
            if (rightNode->size >= (MAX + 1) / 2 + 1) {
                cursor->key[cursor->size] = rightNode->key[0];
                cursor->metadata[cursor->size] = rightNode->metadata[0]; // Asigna la metadata también
                cursor->ptr[cursor->size + 1] = rightNode->ptr[0];
                cursor->size++;
                for (int i = 0; i < rightNode->size - 1; i++) {
                    rightNode->key[i] = rightNode->key[i + 1];
                    rightNode->metadata[i] = rightNode->metadata[i + 1]; // Mueve las metadata también
                }
                rightNode->size--;
                parent->key[rightSibling - 1] = rightNode->key[0];
                return;
            }
        }

        if (leftSibling >= 0) {
            Node *leftNode = parent->ptr[leftSibling];
            if (leftNode->size >= (MAX + 1) / 2 + 1) {
                for (int i = cursor->size; i > 0; i--) {
                    cursor->key[i] = cursor->key[i - 1];
                    cursor->metadata[i] = cursor->metadata[i - 1]; // Mueve las metadata también
                }
                cursor->key[0] = leftNode->key[leftNode->size - 1];
                cursor->metadata[0] = leftNode->metadata[leftNode->size - 1]; // Asigna la metadata también
                cursor->ptr[0] = leftNode->ptr[leftNode->size];
                leftNode->size--;
                cursor->size++;
                parent->key[leftSibling] = cursor->key[0];
                return;
            }
        }

        if (rightSibling <= parent->size) {
            Node *rightNode = parent->ptr[rightSibling];
            for (int i = cursor->size, j = 0; j < rightNode->size; i++, j++) {
                cursor->key[i] = rightNode->key[j];
                cursor->metadata[i] = rightNode->metadata[j]; // Mueve las metadata también
            }
            cursor->ptr[cursor->size] = nullptr;
            cursor->size += rightNode->size;
            cursor->ptr[cursor->size] = rightNode->ptr[rightNode->size];
            removeInternal(parent->key[rightSibling - 1], parent, rightNode);
            delete[] rightNode->key;
            delete[] rightNode->ptr;
            delete[] rightNode->metadata; // Liberamos la memoria de metadata
            delete rightNode;
        } else if (leftSibling >= 0) {
            Node *leftNode = parent->ptr[leftSibling];
            for (int i = leftNode->size, j = 0; j < cursor->size; i++, j++) {
                leftNode->key[i] = cursor->key[j];
                leftNode->metadata[i] = cursor->metadata[j]; // Mueve las metadata también
            }
            leftNode->ptr[leftNode->size] = nullptr;
            leftNode->size += cursor->size;
            leftNode->ptr[leftNode->size] = cursor->ptr[cursor->size];
            removeInternal(parent->key[leftSibling], parent, cursor);
            delete[] cursor->key;
            delete[] cursor->ptr;
            delete[] cursor->metadata; // Liberamos la memoria de metadata
            delete cursor;
        }
    }
}

void BPTree::removeInternal(int x, Node *cursor, Node *child) {
    if (cursor == root) {
        if (cursor->size == 1) {
            if (cursor->ptr[1] == child) {
                delete[] child->key;
                delete[] child->ptr;
                delete[] child->metadata; // Liberamos la memoria de metadata
                delete child;
                root = cursor->ptr[0];
                delete[] cursor->key;
                delete[] cursor->ptr;
                delete[] cursor->metadata; // Liberamos la memoria de metadata
                delete cursor;
                return;
            } else if (cursor->ptr[0] == child) {
                delete[] child->key;
                delete[] child->ptr;
                delete[] child->metadata; // Liberamos la memoria de metadata
                delete child;
                root = cursor->ptr[1];
                delete[] cursor->key;
                delete[] cursor->ptr;
                delete[] cursor->metadata; // Liberamos la memoria de metadata
                delete cursor;
                return;
            }
        }
    }

    int pos;
    for (pos = 0; pos < cursor->size; pos++) {
        if (cursor->key[pos] == x) {
            break;
        }
    }

    for (int i = pos; i < cursor->size - 1; i++) {
        cursor->key[i] = cursor->key[i + 1];
    }
    for (int i = pos + 1; i < cursor->size + 1; i++) {
        cursor->ptr[i] = cursor->ptr[i + 1];
    }
    cursor->size--;

    if (cursor->size >= (MAX + 1) / 2 - 1) {
        return;
    }

    if (cursor == root) return;

    Node *parent = findParent(root, cursor);
    int leftSibling, rightSibling;

    for (pos = 0; pos < parent->size + 1; pos++) {
        if (parent->ptr[pos] == cursor) {
            leftSibling = pos - 1;
            rightSibling = pos + 1;
            break;
        }
    }

    if (rightSibling <= parent->size) {
        Node *rightNode = parent->ptr[rightSibling];
        if (rightNode->size >= (MAX + 1) / 2) {
            cursor->key[cursor->size] = parent->key[rightSibling - 1];
            cursor->ptr[cursor->size + 1] = rightNode->ptr[0];
            parent->key[rightSibling - 1] = rightNode->key[0];
            for (int i = 0; i < rightNode->size - 1; i++) {
                rightNode->key[i] = rightNode->key[i + 1];
            }
            for (int i = 0; i < rightNode->size; i++) {
                rightNode->ptr[i] = rightNode->ptr[i + 1];
            }
            cursor->size++;
            rightNode->size--;
            return;
        }
    }

    if (leftSibling >= 0) {
        Node *leftNode = parent->ptr[leftSibling];
        if (leftNode->size >= (MAX + 1) / 2) {
            for (int i = cursor->size; i > 0; i--) {
                cursor->key[i] = cursor->key[i - 1];
            }
            cursor->key[0] = parent->key[leftSibling];
            for (int i = cursor->size + 1; i > 0; i--) {
                cursor->ptr[i] = cursor->ptr[i - 1];
            }
            cursor->ptr[0] = leftNode->ptr[leftNode->size];
            parent->key[leftSibling] = leftNode->key[leftNode->size - 1];
            cursor->size++;
            leftNode->size--;
            return;
        }
    }

    if (rightSibling <= parent->size) {
        Node *rightNode = parent->ptr[rightSibling];
        for (int i = cursor->size, j = 0; j < rightNode->size; i++, j++) {
            cursor->key[i] = rightNode->key[j];
        }
        for (int i = cursor->size + 1, j = 0; j < rightNode->size + 1; i++, j++) {
            cursor->ptr[i] = rightNode->ptr[j];
        }
        cursor->size += rightNode->size;
        removeInternal(parent->key[rightSibling - 1], parent, rightNode);
        delete[] rightNode->key;
        delete[] rightNode->ptr;
        delete[] rightNode->metadata; // Liberamos la memoria de metadata
        delete rightNode;
    } else if (leftSibling >= 0) {
        Node *leftNode = parent->ptr[leftSibling];
        for (int i = leftNode->size, j = 0; j < cursor->size; i++, j++) {
            leftNode->key[i] = cursor->key[j];
        }
        for (int i = leftNode->size + 1, j = 0; j < cursor->size + 1; i++, j++) {
            leftNode->ptr[i] = cursor->ptr[j];
        }
        leftNode->size += cursor->size;
        removeInternal(parent->key[leftSibling], parent, cursor);
        delete[] cursor->key;
        delete[] cursor->ptr;
        delete[] cursor->metadata; // Liberamos la memoria de metadata
        delete cursor;
    }
}