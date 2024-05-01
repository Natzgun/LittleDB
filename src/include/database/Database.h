//
// Created by natzgun on 18/04/24.
//

#ifndef DATABASE_H
#define DATABASE_H
#include <iostream>

#include "Validator.h"
using namespace std;

class Database {
private:
  string schemaExists(string &tableName);
  string separator(string &lineToSeparate);
  Validator validator;

public:
  Database();

  void createTable(string &command);

  void insertInSchema(string &command);

  void showTables();

  void selectTable(string &tableName, string &columns, string &condition);

  void readCSV(string &command);
};
#endif //DATABASE_H
