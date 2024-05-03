#ifndef DISK_H
#define DISK_H
class Disk {
private:
  int numPlatters;
  int tracksPerSurface;
  int sectorsPerTrack;
  int bytesPerSector;
  int sectorPerBlock;

public:
  Disk();
  Disk(int,int,int,int);
  void capacityDisk();
  void generateDiskStructure();
};
#endif //DISK_H
