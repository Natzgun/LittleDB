#ifndef STORAGE_H
#define STORAGE_H
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Storage {
private:
  int size;
public:
  Storage();
  void initSchema(std::vector<std::string>& data);
  void insertDataSchema();

  void execCommand(string &command);
  void createTable(string &command);
  void showTable();
};



#endif //STORAGE_H
