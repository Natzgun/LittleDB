#ifndef DISK_MANAGER_H
#define DISK_MANAGER_H

#include "storage/HeapFile.h"
#include "storage/Disk.h"
#include "storage/TreeNode.h"
class Disk_manager {
private:
  Disk disk;
  HeapFile heapFile;
  TreeNode& rootNode;
  vector<int> dataDisk;

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

  vector<int> getDataDisk() const;
};
#endif //DISK_MANAGER_H
