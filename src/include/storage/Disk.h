#ifndef DISK_H
#define DISK_H

#include <filesystem>
#include <vector>

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

  void createDirectories(TreeNode& node, int levels, const std::vector<int>& directoriesPerLevel);

public:
  Disk();
  Disk(int,int,int,int);
  void capacityDisk();
  void generateDiskStructure();
};
#endif //DISK_H
