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
public:
  Database();

  void createTable(string &command);

  void insertInSchema(string &command);

  void showTables();

  void selectTable(string &command);

  void readCSV();
};
#endif //DATABASE_H
