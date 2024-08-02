#ifndef DATABASEMED
#define DATABASEMED

#include "buffer/BufferManager.h"
#include "storage/Disk_manager.h"
#include "storage/index/BPlusTree.h"
#include <iostream>
#include <string>

using namespace std;

class DatabaseMediator {
  private:
  void loadBlockMediator(int blockID, char mode);
  int convertPathToPage(const string& path, char mod);
  BufferManager bfManager;
  Disk_manager diskManager;
  map<string,BPlusTree> bPlusTrees;

  BPlusTree& getOrCreateBPTree(string relation);
  string getBlockFromBPtreeForInsert(string key, string relation, bool space = true);
  vector<string> getBlocksForRead(string key, string relation);
  void fillBPtree(string relation);
  void loadBPtree(string relation);
  void saveBPtree(string relation);
  vector<string> split(const string& str, char delimiter);
  bool findRelationInFile(string &linea,const string& relationName,const string& filename = "../../data/usr/db/schemas.txt");
  pair<vector<string>, vector<pair<int,int>>> parseRelation(const string& relationString);
  pair<int,int> setClave(string relation);

public:
  DatabaseMediator();
  void loadIndex();
  void addRecord(string& relation, string record, bool bucle = false, bool end = false);
  void adminRam();
  void selectDiskStructureMediator(bool defaultDisk);
  void saveDataInRAM();
  void loadDataInFiles();
  void adminBplusTree();

  void medSaveBlocksInSectors(string relation);
};

#endif