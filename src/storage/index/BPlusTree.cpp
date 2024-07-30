#include "storage/index/BPlusTree.h"

BPlusTree::BPlusTree(int _maxCapacity) {
    root = new Node(nullptr, true, nullptr, nullptr);
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
        left->parent = right->parent = root = new Node(nullptr, false, nullptr, nullptr);
        depth += 1;
        root->keys = {key};
        root->children = {left, right};
        return;
    }
    parent->setChild(key, {left, right});
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
}

void BPlusTree::borrowKeyFromLeftLeaf(Node* node, Node* prev) {
    node->keys.insert(node->keys.begin(), prev->keys.back());
    node->rutas.insert(node->rutas.begin(), prev->rutas.back());
    prev->keys.pop_back();
    prev->rutas.pop_back();
}

tuple<string, Node*> BPlusTree::mergeInternal(int myPositionInParent, Node* node, Node* next) {
    node->keys.push_back(node->parent->keys[myPositionInParent]);
    node->keys.insert(node->keys.end(), next->keys.begin(), next->keys.end());
    node->children.insert(node->children.end(), next->children.begin(), next->children.end());
    for (Node* child : node->children) {
        child->parent = node;
    }
    return make_tuple(node->keys.back(), node);
}

tuple<string, Node*> BPlusTree::mergeLeaf(Node* node, Node* next) {
    node->keys.insert(node->keys.end(), next->keys.begin(), next->keys.end());
    node->rutas.insert(node->rutas.end(), next->rutas.begin(), next->rutas.end());
    return make_tuple(node->keys.back(), node);
}

void BPlusTree::search(Node* node, const string& key, pair<string, string>& result) {
    if (node == nullptr) return;

    int index = node->indexOfKey(key);
    if (node->isLeaf) {
        if (index != -1) {
            result = node->rutas[index];
        }
        return;
    }

    if (index == -1) index = node->indexOfChild(key);
    search(node->children[index], key, result);
}

pair<string, string> BPlusTree::search(const string& key) {
    pair<string, string> result = {"NullSearch", "NullSearch"};
    Node* leaf = findLeaf(key);
    search(leaf, key, result);
    return result;
}

void BPlusTree::printTreeByLevels() const {
    if (!root) {
        cout << "Tree is empty!" << endl;
        return;
    }

    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        int levelSize = q.size();
        for (int i = 0; i < levelSize; ++i) {
            Node* node = q.front();
            q.pop();
            node->printNode();
            for (Node* child : node->children) {
                if (child) q.push(child);
            }
        }
        cout << "----------" << endl;
    }
}

void BPlusTree::remove(const string& key) {
    Node* leaf = findLeaf(key);
    if (leaf == nullptr) {
        cout << "Key " << key << " not found! Exiting ..." << endl;
        return;
    }

    removeFromLeaf(key, leaf);

    if (leaf->keys.size() < minCapacity) {
        handleUnderflow(leaf);
    }
}

void BPlusTree::handleUnderflow(Node* node) {
    if (node == root) {
        if (node->isLeaf && node->keys.empty()) {
            delete root;
            root = nullptr;
        }
        return;
    }

    Node* parent = node->parent;
    int positionInParent = parent->indexOfChild(node->keys.front());

    if (node->isLeaf) {
        if (positionInParent > 0) {
            Node* leftSibling = parent->children[positionInParent - 1];
            if (leftSibling->keys.size() > minCapacity) {
                borrowKeyFromLeftLeaf(node, leftSibling);
                return;
            }
        }

        if (positionInParent < parent->keys.size()) {
            Node* rightSibling = parent->children[positionInParent + 1];
            if (rightSibling->keys.size() > minCapacity) {
                borrowKeyFromRightLeaf(node, rightSibling);
                return;
            }
        }

        if (positionInParent > 0) {
            Node* leftSibling = parent->children[positionInParent - 1];
            mergeLeaf(leftSibling, node);
            handleUnderflow(leftSibling);
        } else {
            Node* rightSibling = parent->children[positionInParent + 1];
            mergeLeaf(node, rightSibling);
            handleUnderflow(node);
        }
    } else {
        if (positionInParent > 0) {
            Node* leftSibling = parent->children[positionInParent - 1];
            if (leftSibling->keys.size() > minCapacity) {
                borrowKeyFromLeftInternal(positionInParent, node, leftSibling);
                return;
            }
        }

        if (positionInParent < parent->keys.size()) {
            Node* rightSibling = parent->children[positionInParent + 1];
            if (rightSibling->keys.size() > minCapacity) {
                borrowKeyFromRightInternal(positionInParent, node, rightSibling);
                return;
            }
        }

        if (positionInParent > 0) {
            Node* leftSibling = parent->children[positionInParent - 1];
            string newParentKey;
            tie(newParentKey, leftSibling) = mergeInternal(positionInParent - 1, leftSibling, node);
            parent->keys[positionInParent - 1] = newParentKey;
            handleUnderflow(leftSibling);
        } else {
            Node* rightSibling = parent->children[positionInParent + 1];
            string newParentKey;
            tie(newParentKey, rightSibling) = mergeInternal(positionInParent, node, rightSibling);
            parent->keys[positionInParent] = newParentKey;
            handleUnderflow(rightSibling);
        }
    }
}

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

pair<string, string> BPlusTree::searchPolitica(Node* root, string key) {
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
    string path = getData.first; // Ajusta esto según tu implementación
    string offset = getData.second; // Ajusta esto según tu implementación
    return {path, offset};
}

Node* BPlusTree::getRoot() const {
    return root;
}