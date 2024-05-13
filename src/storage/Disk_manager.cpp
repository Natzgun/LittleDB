#include "storage/Disk_manager.h"

#include <iostream>

using namespace std;

void Disk_manager::selectDiskStructure(bool defaultDisk) {
  if (defaultDisk) {
    disk.generateDiskStructure();
    disk.capacityDisk();
  } else {
    int plates, tracks, sector, bytes, bytesPerBlock;
    cout << "Ingrese el número de platos: ";
    cin >> plates;
    cout << "Ingrese el número de pistas por superficie: ";
    cin >> tracks;
    cout << "Ingrese el número de sectores por pista: ";
    cin >> sector;
    cout << "Ingrese el número de bytes por sector: ";
    cin >> bytes;
    cout << "Ingrese el numero de bytes por bloque: ";
    cin >> bytesPerBlock;
    disk = Disk(plates, tracks, sector, bytes, bytesPerBlock);
    disk.generateDiskStructure();
    disk.capacityDisk();
  }
}

void Disk_manager::getCapacityDisk() {
  disk.capacityDisk();
}
