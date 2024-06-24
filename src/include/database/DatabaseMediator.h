#ifndef DATABASEMED
#define DATABASEMED

#include "buffer/BufferManager.h"
#include "storage/Disk_manager.h"
#include <iostream>
#include <string>

using namespace std;

class mediator {
  private:
    BufferManager buffManager;
    Disk_manager disManager;
  public:
    void addRecord(string& relation, string record);
};

#endif