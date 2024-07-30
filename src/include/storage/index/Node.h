
#ifndef NODE_H
#define NODE_H
#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <algorithm>
using namespace std;


class Node {
public:
  vector<string> keys;
  Node* parent;
  vector<Node*> children;
  Node* next;
  Node* prev;
  bool isLeaf;
  vector<pair<string, string>> rutas;

  Node(Node* parent = nullptr, bool isLeaf = false, Node* prev_ = nullptr, Node* next_ = nullptr);
  int getNumKeys() const;
  int indexOfChild(const string& key) const;
  int indexOfKey(const string& key) const;
  Node* getChild(const string& key);
  void setChild(const string& key, const vector<Node*>& value);
  tuple<string, Node*, Node*> splitInternal();
  void set(const string& key, const pair<string, string>& ruta);
  tuple<string, Node*, Node*> splitLeaf();
  pair<string,string> getMetadata(int pos);
  void printNode() const;
};

#endif //NODE_H
