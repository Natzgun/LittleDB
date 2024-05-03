#include "storage/Disk.h"
#include <iostream>
using namespace std;

Disk::Disk() {
  this->numPlatters = 8;
  this->tracksPerSurface = 16384;
  this->sectorsPerTrack = 128;
  this->bytesPerSector = 4096;
}

Disk::Disk(int plates, int tracks, int sector, int bytes) : numPlatters(plates), tracksPerSurface(tracks), sectorsPerTrack(sector), bytesPerSector(bytes){
}

void Disk::capacityDisk() {
  cout << "Disk capacity: " << numPlatters * 2 * tracksPerSurface * sectorsPerTrack * bytesPerSector << " bytes" << endl;
}

void Disk::generateDiskStructure() {
}
