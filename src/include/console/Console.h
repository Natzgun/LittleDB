#ifndef CONSOLE_H
#define CONSOLE_H
#include "database/Database.h"
#include "storage/Storage.h"

class Console {
private:
  Database& db;
  string tableName;
  string columns;
  string condition;
  void execCommand(string &command);

  void selectDisk();

public:
  Console(Database& db);
  void run();

  void parseSelect(string &selectCommand);
  void cleanCommands();
};
#endif //CONSOLE_H
