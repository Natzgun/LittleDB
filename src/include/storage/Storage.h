#ifndef STORAGE_H
#define STORAGE_H
#include <iostream>

using namespace std;

class Storage {
private:
  int size;
public:
  Storage();
  void insertDataSchema();

  void createTable(string &command);
  void showTable();
};



#endif //STORAGE_H
