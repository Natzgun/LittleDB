#include "database/Query.h"

#include <algorithm>
#include <fstream>
#include <sstream>

void Query::selectAllColumns(string &tableName) {
  ifstream inFile("../../data/usr/db/" + tableName + ".txt");
  if (!inFile.is_open()) {
    cerr << "Error: No se pudo abrir el archivo de la tabla '" << tableName << "'." << endl;
    return;
  }

  string line;
  while (getline(inFile, line)) {
    replace(line.begin(), line.end(), '#', '\t');
    cout << line << endl;
  }

  inFile.close();
}

void Query::selectColumn(string &tableName, int columnIndex) {

    ifstream inFile("../../data/usr/db/" + tableName + ".txt");
    if (!inFile.is_open()) {
      cerr << "Error: No se pudo abrir el archivo de la tabla '" << tableName << "'." << endl;
      return;
    }

    string line;
    while (getline(inFile, line)) {
      stringstream ss(line);
      string token;
      int currentColumn = 0;
      while (getline(ss, token, '#')) {
        if (currentColumn == columnIndex) {
          cout << token << endl;
          break;
        }
        ++currentColumn;
      }
    }

    inFile.close();
}
