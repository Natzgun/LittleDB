#ifndef DATABASEMED
#define DATABASEMED

#include "buffer/BufferManager.h"
#include "storage/Disk_manager.h"
#include <iostream>
#include <string>

using namespace std;

class DatabaseMediator {
  private:
    void loadBlockMediator(int blockID, char mode);

    BufferManager bfManager;
    Disk_manager diskManager;
  public:
  DatabaseMediator();
    void addRecord(string& relation, string record, bool bucle = false, bool end = false);
    void adminRam();
    void selectDiskStructureMediator(bool defaultDisk);
    void saveDataInRAM();
    void loadDataInFiles();
};

#endif