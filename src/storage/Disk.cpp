#include "storage/Disk.h"

#include <cstdint>
#include <iostream>
using namespace std;

void Disk::createDirectories(TreeNode &node, int levels, const std::vector<int> &directoriesPerLevel) {
  if (levels <= 0)
    return;

  try {
    fs::create_directory(node.directory);
    std::cout << "Directorio creado correctamente: " << node.directory << std::endl;

    for (int i = 0; i < directoriesPerLevel[levels - 1]; ++i) {
      TreeNode child;
      child.directory = node.directory / ("dir" + std::to_string(i + 1));
      node.children.push_back(child);
      createDirectories(node.children.back(), levels - 1, directoriesPerLevel);
    }
  } catch (const std::filesystem::filesystem_error &e) {
    std::cerr << "Error al crear el directorio: " << e.what() << std::endl;
  }
}

Disk::Disk() {
  this->numPlatters = 8;
  this->numPlatters = 8;
  this->tracksPerSurface = 16384;
  this->sectorsPerTrack = 128;
  this->bytesPerSector = 4096;

  this->bytesPerBlock = 8192;
  this->sectorPerBlock = this->bytesPerBlock / this->bytesPerSector;
  this->blockPerTrack = this->sectorsPerTrack / (this->bytesPerBlock / this->bytesPerSector);
}

Disk::Disk(int plates, int tracks, int sector, int bytes) : numPlatters(plates), tracksPerSurface(tracks),
                                                            sectorsPerTrack(sector), bytesPerSector(bytes) {
}

void Disk::capacityDisk() {
  long long cap = (this->numPlatters * 2LL) * this->tracksPerSurface * this->sectorsPerTrack * this->bytesPerSector;
  cout << "Disk capacity: " << cap << " bytes [MEGATRON-default]" << endl;
  cout << "Numero de bloques: " << blockPerTrack << endl;
  cout << "Numero de sectores por bloque: " << sectorPerBlock << endl;
}

void Disk::generateDiskStructure() {
  cout << "Se ha generado el disco" << endl;
}
