#ifndef HEAPFILE_H
#define HEAPFILE_H

#include <string>
#include <list>
#include <fstream>
#include <iostream>

using namespace std;

class HeapFile {
private:
  string relation;
  list<string> blocks;

public:
  HeapFile(string relation);

  void addBlock(string block);

  list<string> getBlocks();

  void saveToFile();

  void loadFromFile();

  string getRelation() const;
};

#endif //HEAPFILE_H