#ifndef CONSOLE_H
#define CONSOLE_H
#include "storage/Storage.h"

class Console {
private:
  Storage *storage;
public:
  Console();
  void run();
  void createTable();
};
#endif //CONSOLE_H
