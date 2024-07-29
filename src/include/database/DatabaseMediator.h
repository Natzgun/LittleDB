#ifndef DATABASEMED
#define DATABASEMED

#include "buffer/BufferManager.h"
#include "storage/Disk_manager.h"
#include "storage/index/BPTree.h"
#include <iostream>
#include <string>

using namespace std;

class DatabaseMediator {
  private:
  void loadBlockMediator(int blockID, char mode);
    int convertPathToPage(const string& path, char mod);
  BufferManager bfManager;
  Disk_manager diskManager;
  map<string,BPTree> bPlusTrees;

  BPTree& getOrCreateBPTree(string relation);
  string getBlockFromBPtreeForInsert(string key, string relation);
  vector<string> getBlocksForRead(string key, string relation);
  void fillBPtree(string relation);
  void loadBPtree(string relation);

  public:
  DatabaseMediator();
  void addRecord(string& relation, string record, bool bucle = false, bool end = false);
  void adminRam();
  void selectDiskStructureMediator(bool defaultDisk);
  void saveDataInRAM();
  void loadDataInFiles();
};

#endif