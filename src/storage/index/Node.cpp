#include "storage/index/Node.h"
#include "storage/index/Metadata.h"

Node::Node(Node* parent, bool isLeaf, Node* prev_, Node* next_)
    : parent(parent), isLeaf(isLeaf), prev(prev_), next(next_) {
    if (next_) next_->prev = this;
    if (prev_) prev_->next = this;
}

int Node::indexOfChild(const string& key) const {
    for (int i = 0; i < keys.size(); i++) {
        if (key < keys[i]) return i;
    }
    return keys.size();
}

int Node::indexOfKey(const string& key) const {
    for (int i = 0; i < keys.size(); i++) {
        if (key == keys[i]) return i;
    }
    return -1;
}

Node* Node::getChild(const string& key) {
    return children[indexOfChild(key)];
}

void Node::setChild(const string& key, const vector<Node*>& value) {
    int i = indexOfChild(key);
    keys.insert(keys.begin() + i, key);
    children.erase(children.begin() + i);
    children.insert(children.begin() + i, value.begin(), value.end());
}

tuple<string, Node*, Node*> Node::splitInternal() {
    Node* left = new Node(parent, false, nullptr, nullptr);
    int mid = keys.size() / 2;

    copy(keys.begin(), keys.begin() + mid, back_inserter(left->keys));
    copy(children.begin(), children.begin() + mid + 1, back_inserter(left->children));

    for (Node* child : left->children) {
        child->parent = left;
    }

    string key = keys[mid];
    keys.erase(keys.begin() + mid);
    children.erase(children.begin() + mid + 1);

    return make_tuple(key, left, this);
}

void Node::set(const string& key, const pair<string, string>& ruta) {
    int i = indexOfChild(key);
    if (find(keys.begin(), keys.end(), key) == keys.end()) {
        keys.insert(keys.begin() + i, key);
        if (isLeaf) {
            rutas.insert(rutas.begin() + i, ruta);
        }
    }
}

tuple<string, Node*, Node*> Node::splitLeaf() {
    Node* left = new Node(parent, true, prev, this);
    int mid = keys.size() / 2;

    left->keys.assign(keys.begin(), keys.begin() + mid);
    left->rutas.assign(rutas.begin(), rutas.begin() + mid);

    keys.erase(keys.begin(), keys.begin() + mid);
    rutas.erase(rutas.begin(), rutas.begin() + mid);

    return make_tuple(keys[0], left, this);
}

void Node::printNode() const {
    cout << "Node keys: ";
    for (const auto& key : keys) cout << key << " ";
    cout << endl;
    /*
    if (isLeaf) {
        cout << "Leaf rutas: ";
        for (const auto& ruta : rutas) cout << "[" << ruta.first << ", " << ruta.second << "] ";
        cout << endl;
    }

    cout << "Children: ";
    for (const auto& child : children) cout << (child ? "[child] " : "[null] ");
    cout << endl;
    */
}