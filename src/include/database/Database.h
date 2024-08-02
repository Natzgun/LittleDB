//
// Created by natzgun on 18/04/24.
//

#ifndef DATABASE_H
#define DATABASE_H
#include <iostream>

#include "DatabaseMediator.h"
#include "Validator.h"
#include "storage/Disk_manager.h"
using namespace std;

class Database {
private:
  string schemaExists(string &tableName);
  string separator(string &lineToSeparate);
  int getColumnIndex(string &tableName, string &columns);
  int extractFixedLenghtRecord(string &relation);

  /* Estas dos funciones implementan Longitud Fija y Variable*/
  string formatRecordRLF(string &relationSchema, string &record);
  string formatRecordRLV(string &relationSchema, string &record);

  Validator validator;
  DatabaseMediator dbMediator;
public:
  Database();

  void createTable(string &command);

  void insertInSchema(string &command);

  void selectTable(string &tableName, string &columns, string &condition);

  void readCSV(string &command);

  void selectDisk(bool defaultDisk);

  void viewRam();

  void saveDataInRAM();

  void loadDataInFiles();

  void adminBtree();

  void saveSectors(string relation);

  void querys();
};
#endif //DATABASE_H
