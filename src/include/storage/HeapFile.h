#ifndef HEAPFILE_H
#define HEAPFILE_H

#include <string>
#include <list>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

class HeapFile {
private:
  string pathHeapFile;
  vector<string> freeBlocks;

public:
  HeapFile(string pathHF);

  void addFreeBlock(string block);

  vector<string> getBlocks();

  void saveToFileFreeBlocks();

  void loadFromFileFreeBlocks();

  static void addBlockToRelation(string relation, string block);

  string getPathHF() const;
};

#endif //HEAPFILE_H