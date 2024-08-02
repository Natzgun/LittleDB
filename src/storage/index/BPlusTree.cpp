#include "storage/index/BPlusTree.h"

BPlusTree::BPlusTree(int _maxCapacity) {
    root = new Node(nullptr, true);
    maxCapacity = _maxCapacity;
    minCapacity = maxCapacity / 2;
    depth = 0;
}

void BPlusTree::writeNode(Node* node, ofstream& outFile) const {
    if (node == nullptr) return;

    outFile << "  node" << node << " [label=\"";
    for (size_t i = 0; i < node->keys.size(); ++i) {
        if (node->isLeaf) {
            outFile << node->keys[i] << "\\n" << node->rutas[i].first << "\\n" << node->rutas[i].second;
        } else {
            outFile << node->keys[i];
        }
        if (i < node->keys.size() - 1) outFile << "|";
    }
    outFile << "\"];\n";

    for (size_t i = 0; i < node->children.size(); ++i) {
        Node* child = node->children[i];
        if (child) {
            outFile << "  node" << node << " -> node" << child << ";\n";
            writeNode(child, outFile);
        }
    }
}

void BPlusTree::exportToDot(const string& filename) const {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }

    outFile << "digraph BPlusTree {\n";
    outFile << "  node [shape=record];\n";
    writeNode(root, outFile);
    outFile << "}\n";
    outFile.close();
}

Node* BPlusTree::findLeaf(const string& key) {
    Node* node = root;
    while (!node->isLeaf) node = node->getChild(key);
    return node;
}

void BPlusTree::set(const string& key, const pair<string, string>& ruta) {
    Node* leaf = findLeaf(key);
    leaf->set(key, ruta);
    if (leaf->keys.size() > maxCapacity) insert(leaf->splitLeaf());
}

void BPlusTree::insert(tuple<string, Node*, Node*> result) {
    string key = get<0>(result);
    Node* left = get<1>(result);
    Node* right = get<2>(result);

    Node* parent = right->parent;

    if (parent == nullptr) {
        left->parent = right->parent = root = new Node(nullptr, false);
        depth += 1;
        root->keys = {key};
        root->children = {left, right};
        return;
    }
    parent->setChild(key, left, right);
    if (parent->keys.size() > maxCapacity) insert(parent->splitInternal());
}

void BPlusTree::removeFromLeaf(const string& key, Node* node) {
    int index = node->indexOfKey(key);
    if (index == -1) {
        cout << "Key " << key << " not found! Exiting ..." << endl;
        exit(0);
    }
    node->keys.erase(node->keys.begin() + index);
    node->rutas.erase(node->rutas.begin() + index);
    if (node->parent) {
        int indexInParent = node->parent->indexOfChild(key);
        if (indexInParent > 0) node->parent->keys[indexInParent - 1] = node->keys.front();
    }
}

void BPlusTree::removeFromInternal(const string& key, Node* node) {
    int index = node->indexOfKey(key);
    if (index != -1) {
        Node* leftMostLeaf = node->children[index + 1];
        while (!leftMostLeaf->isLeaf) leftMostLeaf = leftMostLeaf->children.front();
        node->keys[index] = leftMostLeaf->keys.front();
    }
}

void BPlusTree::borrowKeyFromRightInternal(int myPositionInParent, Node* node, Node* next) {
    node->keys.push_back(node->parent->keys[myPositionInParent]);
    node->parent->keys[myPositionInParent] = next->keys.front();
    next->keys.erase(next->keys.begin());
    node->children.insert(node->children.end(), next->children.begin(), next->children.end());
    for (Node* child : node->children) {
        child->parent = node;
    }
}

void BPlusTree::borrowKeyFromLeftInternal(int myPositionInParent, Node* node, Node* prev) {
    node->keys.insert(node->keys.begin(), node->parent->keys[myPositionInParent - 1]);
    node->parent->keys[myPositionInParent - 1] = prev->keys.back();
    prev->keys.erase(prev->keys.end() - 1);
    node->children.insert(node->children.begin(), prev->children.back());
    prev->children.pop_back();
    node->children.front()->parent = node;
}

void BPlusTree::borrowKeyFromRightLeaf(Node* node, Node* next) {
    node->keys.push_back(next->keys.front());
    node->rutas.push_back(next->rutas.front());
    next->keys.erase(next->keys.begin());
    next->rutas.erase(next->rutas.begin());
    int myPositionInParent = node->parent->indexOfChild(node->keys.front());
    node->parent->keys[myPositionInParent] = next->keys.front();
}

void BPlusTree::borrowKeyFromLeftLeaf(Node* node, Node* prev) {
    node->keys.insert(node->keys.begin(), prev->keys.back());
    node->rutas.insert(node->rutas.begin(), prev->rutas.back());
    prev->keys.erase(prev->keys.end() - 1);
    prev->rutas.erase(prev->rutas.end() - 1);
    int myPositionInParent = node->parent->indexOfChild(node->keys.front());
    node->parent->keys[myPositionInParent - 1] = node->keys.front();
}

void BPlusTree::mergeWithRightInternal(int myPositionInParent, Node* node, Node* next) {
    node->keys.push_back(node->parent->keys[myPositionInParent]);
    node->keys.insert(node->keys.end(), next->keys.begin(), next->keys.end());
    node->children.insert(node->children.end(), next->children.begin(), next->children.end());
    for (Node* child : node->children) {
        child->parent = node;
    }
    node->parent->keys.erase(node->parent->keys.begin() + myPositionInParent);
    node->parent->children.erase(node->parent->children.begin() + myPositionInParent + 1);
    delete next;
    if (node->parent->keys.empty()) {
        if (node->parent == root) {
            root = node;
            node->parent = nullptr;
            depth--;
        } else {
            merge(node->parent);
        }
    }
}

void BPlusTree::mergeWithRightLeaf(Node* node, Node* next) {
    node->keys.insert(node->keys.end(), next->keys.begin(), next->keys.end());
    node->rutas.insert(node->rutas.end(), next->rutas.begin(), next->rutas.end());
    node->next = next->next;
    if (next->next) next->next->prev = node;
    int myPositionInParent = node->parent->indexOfChild(node->keys.front());
    node->parent->keys.erase(node->parent->keys.begin() + myPositionInParent);
    node->parent->children.erase(node->parent->children.begin() + myPositionInParent + 1);
    delete next;
    if (node->parent->keys.empty()) {
        if (node->parent == root) {
            root = node;
            node->parent = nullptr;
            depth--;
        } else {
            merge(node->parent);
        }
    }
}

void BPlusTree::merge(Node* node) {
    if (!node->parent) return;

    int myPositionInParent = node->parent->indexOfChild(node->keys.front());
    Node* prev = (myPositionInParent > 0) ? node->parent->children[myPositionInParent - 1] : nullptr;
    Node* next = (myPositionInParent < node->parent->children.size() - 1) ? node->parent->children[myPositionInParent + 1] : nullptr;

    if (node->isLeaf) {
        if (next && next->keys.size() > minCapacity) {
            borrowKeyFromRightLeaf(node, next);
        } else if (prev && prev->keys.size() > minCapacity) {
            borrowKeyFromLeftLeaf(node, prev);
        } else if (next) {
            mergeWithRightLeaf(node, next);
        } else if (prev) {
            mergeWithRightLeaf(prev, node);
        }
    } else {
        if (next && next->keys.size() > minCapacity) {
            borrowKeyFromRightInternal(myPositionInParent, node, next);
        } else if (prev && prev->keys.size() > minCapacity) {
            borrowKeyFromLeftInternal(myPositionInParent, node, prev);
        } else if (next) {
            mergeWithRightInternal(myPositionInParent, node, next);
        } else if (prev) {
            mergeWithRightInternal(myPositionInParent - 1, prev, node);
        }
    }
}

void BPlusTree::remove(const string& key) {
    Node* node = findLeaf(key);
    removeFromLeaf(key, node);
    if (node->keys.size() < minCapacity) merge(node);
}

Node * BPlusTree::getRoot() {
    return root;
}

void BPlusTree::printTree() {
    if (!root) {
        cout << "The tree is empty." << endl;
        return;
    }

    queue<Node*> nodesQueue;
    nodesQueue.push(root);

    while (!nodesQueue.empty()) {
        int levelSize = nodesQueue.size();

        while (levelSize > 0) {
            Node* node = nodesQueue.front();
            nodesQueue.pop();

            cout << "[";
            for (const auto& key : node->keys) {
                cout << key << " ";
            }
            cout << "] ";

            for (Node* child : node->children) {
                if (child) {
                    nodesQueue.push(child);
                }
            }

            levelSize--;
        }
        cout << endl;
    }
}

pair<string, string> BPlusTree::search(const string &key) {
    Node* leaf = findLeaf(key);
    int index = leaf->indexOfKey(key);
    if (index != -1) {
        return leaf->rutas[index];
    }
    return {"NullSearch", "NullSearch"};
}

// Search de Ramos
pair<Node*, int> BPlusTree::searchNode(Node* root, string key) {
    Node* current = this -> root;
    while (!current->isLeaf) {
        int i;
        for (i = 0; i < current->getNumKeys(); i++) {
            if (key < current->keys[i]) {
                break;
            }
        }
        current = current->children[i];
    }
    // Encontrar la posición exacta en la hoja
    int pos;
    for (pos = 0; pos < current->getNumKeys(); pos++) {
        if (key <= current->keys[pos]) {
            break;
        }
    }
    return {current, pos};
}

pair<string, string> BPlusTree::searchPolicy(Node* root, string key) {
    auto [leaf, pos] = searchNode(root, key);
    // Si encontramos la clave exacta
    pair<string,string> result = search(key);
    if (result.first != "NullSearch") {
        return result;
    }
    if (pos != 0) {
        pos --;
    }
    auto getData = leaf -> getMetadata(pos);
    string path = getData.first;
    string offset = getData.second;
    return {path, offset};
}

void processFile(string fileName, BPlusTree& bpTree) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error: No se pudo abrir el archivo " << fileName << endl;
        return;
    }

    string line;
    int count = 0;
    while (getline(file, line)) {
        istringstream ss(line);
        string palabra;
        string ruta;
        getline(ss, palabra, ',');
        getline(ss, ruta, ',');
        bpTree.set(palabra, {palabra, ruta});
        count++;
    }

    cout << "Total de entradas procesadas: " << count << endl;
}
