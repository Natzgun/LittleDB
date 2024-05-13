//
// Created by natzgun on 18/04/24.
//

#ifndef DATABASE_H
#define DATABASE_H
#include <iostream>

#include "Validator.h"
#include "storage/Disk_manager.h"
using namespace std;

class Database {
private:
  string schemaExists(string &tableName);
  string separator(string &lineToSeparate);
  int getColumnIndex(string &tableName, string &columns);
  int extractFixedLenghtRecord(string &relation);

  string formatRecord(string &relation, string &record);

  Validator validator;
  Disk_manager diskManager;
public:
  Database();

  void createTable(string &command);

  void insertInSchema(string &command);

  void selectTable(string &tableName, string &columns, string &condition);

  void readCSV(string &command);

  void selectDisk(bool defaultDisk);
};
#endif //DATABASE_H
