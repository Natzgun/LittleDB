#ifndef DISK_MANAGER_H
#define DISK_MANAGER_H

#include <map>

#include "storage/HeapFile.h"
#include "storage/Disk.h"
#include "storage/TreeNode.h"
#include <map>
class Disk_manager {
private:
  Disk disk;
  HeapFile heapFile;
  TreeNode& rootNode;
  vector<int> dataDisk;
  map<string, map<string,int>> mapOfRelationHF;
public:
  Disk_manager();
  void selectDiskStructure(bool);
  void getCapacityDisk();
  void insertRecord(string &relation, string &record, int recordSize);

  void findFreeBlock();

  void searchFreeBlockInTree(const TreeNode &node);

  bool isBlockFree(const fs::path &blockPath);

  void writeRecordInSectors(string &relation);

  string openBlock(const string &blockPath, const string &relationName);

  bool checkSpaceInBlock(const string &blockPath, int recordSize);

  string redirectSectorWithSpace(const string &blockPath, int recordSize);

  vector<int> getDataDisk();

  void setBlockToRelation(const string &relation);

  void saveFreeBlocks();

  void saveDiskAttributesToFile(string filename = "disk_attributes.txt");

  void saveMapOfRelationHF();

  void loadDiskAttributesFromFile(string filename = "disk_attributes.txt");

  vector<string> splitCapacityAndPath(string blockPath);

  void fillMapOfRelation(const string &relation);

  string getBlockToTree(const string relation);

  void updateMapOfRelationHF(const string &relation, const string &blockPath, int updateSlot);

  vector<string> getBlockPaths(string filePath);
  vector<string> identifySectorsForBlock(string blockPath);
  void distributeBlocksInSectors(string relation);
};
#endif //DISK_MANAGER_H
