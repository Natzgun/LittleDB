#ifndef DISK_MANAGER_H
#define DISK_MANAGER_H

#include "storage/Disk.h"
class Disk_manager {
private:
  Disk disk;
public:

  void selectDiskStructure(bool);
  void getCapacityDisk();

};
#endif //DISK_MANAGER_H
