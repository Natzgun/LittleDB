#include "storage/Storage.h"
#include <fstream>
#include <iostream>

using namespace std;

Storage::Storage(): size(0) {
}

void Storage::initSchema(vector<string> &data) {
  ofstream db_schemas("../../data/usr/schemas.txt", ios::app);
  if (!db_schemas.is_open()) {
    cerr << "Error al abrir el archivo schemmas" << std::endl;
    exit(1);
  }

  for (int i = 0; i < data.size(); ++i) {
    db_schemas << data[i];
    if (i != data.size() - 1) {
      db_schemas << "#";
    }
  }
  db_schemas << endl;
  db_schemas.close();
}
