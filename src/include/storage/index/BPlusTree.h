#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#include "Node.h"

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

class BPlusTree {
    void writeNode(Node* node, ofstream& outFile) const;
    Node* root;
    int maxCapacity;
    int minCapacity;
    int depth;
    pair<Node*, int> searchNode(Node* root, string key);
    
public:

    BPlusTree(int _maxCapacity = 4);
    Node* getRoot() const;
    void exportToDot(const string& filename) const;
    Node* findLeaf(const string& key);
    void set(const string& key, const pair<string, string>& ruta);
    void insert(tuple<string, Node*, Node*> result);
    void removeFromLeaf(const string& key, Node* node);
    void removeFromInternal(const string& key, Node* node);
    void borrowKeyFromRightInternal(int myPositionInParent, Node* node, Node* next);
    void borrowKeyFromLeftInternal(int myPositionInParent, Node* node, Node* prev);
    void borrowKeyFromRightLeaf(Node* node, Node* next);
    void borrowKeyFromLeftLeaf(Node* node, Node* prev);
    tuple<string, Node*> mergeInternal(int myPositionInParent, Node* node, Node* next);
    tuple<string, Node*> mergeLeaf(Node* node, Node* next);
    void search(Node* node, const string& key, pair<string, string>& result);
    pair<string, string> searchPolitica(Node* root, string key);
    pair<string, string> search(const string& key);
    void printTreeByLevels() const;
    void remove(const string& key);
    void handleUnderflow(Node* node);
};

#endif //BPLUSTREE_H
/*
int main() {
    BPlusTree tree(3);

    // Insert example data
    tree.set("key4", {"path1", "value1"});
    tree.set("key2", {"path2", "value2"});
    tree.set("key3", {"path3", "value3"});
    tree.set("key1", {"path3", "value3"});

    // Export tree to DOT file
    tree.exportToDot("tree.dot");

    tree.remove("key3");
    //tree.remove("key2");
    // Print tree by levels
    tree.printTreeByLevels();


    // Search for a key
    auto result = tree.search("key2");
    cout << "Found: " << result.first << ", " << result.second << endl;

    return 0;
}
*/