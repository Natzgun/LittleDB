#ifndef DISK_H
#define DISK_H

#include <filesystem>
#include <vector>
#include <string>
#include "storage/TreeNode.h"
using namespace std;

using std::string;

namespace fs = std::filesystem;

class Disk {
private:
  int numPlatters;
  int tracksPerSurface;
  int sectorsPerTrack;
  int bytesPerSector;

  int sectorPerBlock;
  int bytesPerBlock;
  int blockPerTrack;
  vector<int> datos;
  TreeNode root;

  void createFile(const string & path_file);
  void createFileBlock(const string & path_file, int numberOfBlock, int &numSector);

  void createDirectories(TreeNode& node, int levels, const std::vector<int>& directoriesPerLevel);

  string nameForFiles(int);
public:
  Disk();
  Disk(int,int,int,int,int);
  TreeNode& getRoot();
  void capacityDisk();
  void generateDiskStructure();
  int getNumOfSectors() const;
  int getNumOfPlatters() const;
  vector<int> getDatosDisk();
};
#endif //DISK_H
