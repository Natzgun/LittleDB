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

public:
    Node* root;
    int maxCapacity;
    int minCapacity;
    int depth;

    BPlusTree(int _maxCapacity = 4);

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
    void mergeWithRightInternal(int myPositionInParent, Node* node, Node* next);
    void mergeWithRightLeaf(Node* node, Node* next);
    void merge(Node* node);
    void remove(const string& key);
    Node* getNextLeaf(Node* currentLeaf);

    Node* getRoot();
    void printTree();
    pair<string, string> search(const string& key);

    pair<Node *, int> searchNode(Node *root, string key);
    pair<string, string> searchPolicy(Node *root, string key);
    void updateMetadata(Node *root, string key, string block, string offset);
    vector<std::pair<std::string, std::string>> collectMetadata();
    vector<pair<string, string>> collectMetadataMoreFromKey(const std::string& key);
};

void processFile(string fileName, BPlusTree& bpTree);

#endif // BPLUSTREE_H