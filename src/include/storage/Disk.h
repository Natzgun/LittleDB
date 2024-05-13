#ifndef DISK_H
#define DISK_H

#include <filesystem>
#include <vector>
#include <string>

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

  struct TreeNode {
    fs::path directory;
    std::vector<TreeNode> children;
  };

  TreeNode root;

  void createFile(const string & path_file);
  void createFileBlock(const string & path_file, int numberOfBlock, int &numSector);

  void createDirectories(TreeNode& node, int levels, const std::vector<int>& directoriesPerLevel);

public:
  Disk();
  Disk(int,int,int,int,int);
  TreeNode& getRoot();
  void capacityDisk();
  void generateDiskStructure();
  string nameForFiles(int);
};
#endif //DISK_H
