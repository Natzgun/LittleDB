#ifndef CONSOLE_H
#define CONSOLE_H
#include "database/Database.h"
#include "storage/Storage.h"

class Console {
private:
  Database& db;
  void execCommand(string &command);
public:
  Console(Database& db);
  void run();
  void createTable();
};
#endif //CONSOLE_H
