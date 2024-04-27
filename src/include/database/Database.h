//
// Created by natzgun on 18/04/24.
//

#ifndef DATABASE_H
#define DATABASE_H
#include <iostream>
#include <vector>
using namespace std;

class Database {
private:
  string separator(string &lineToSeparate);

public:
  Database();

  void createTable(string &command);

  void insertInSchema(string &command);

  void showTables();

  void selectTable(string &command);

  void readCSV(string &command);
};
#endif //DATABASE_H
