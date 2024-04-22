#include "database/Database.h"

#include <algorithm>
#include <fstream>
#include <sstream>

string Database::separator(string &lineToSeparate) {
  /*
  short cont = 0;
  while (lineToSeparate)
  */
  return "Todavia falto";
}

Database::Database() {
}

void Database::createTable(string &command) {
  // std::ofstream outFile("../../data/usr/db/schemas.txt", std::ios::app);
  size_t pos1 = command.find("create table ");
  if (pos1 == string::npos) {
    cerr << "Comando inválido para crear tabla." << endl;
    return;
  }
  cout << "Pos 1: " << pos1 << endl;
  size_t pos2 = command.find('(');
  if (pos2 == string::npos) {
    cerr << "Falta especificación de columnas." << endl;
    return;
  }
  cout << "Pos 2: " << pos2 << endl;
  string tableName = command.substr(pos1 + 13, pos2 - 13);

  string columnsDef = command.substr(pos2 + 1, command.size() - pos2 - 3);
  cout << "Comand size: " << command.size() << endl;
  cout << "Col def: " << columnsDef << endl;

  replace(columnsDef.begin(), columnsDef.end(), ' ', ',');

  stringstream output;
  output << tableName << "#";

  // Esto solo es paa itear sobre las columnas y sus tipos
  size_t start = 0, end;
  while ((end = columnsDef.find(',', start)) != string::npos) {
    string columnDef = columnsDef.substr(start, end - start);
    output << columnDef << "#";
    start = end + 1;
  }
  string lastColumn = columnsDef.substr(start);
  output << lastColumn;

  ofstream outFile("../../data/usr/db/schemas.txt", ios::app);
  if (outFile.is_open()) {
    outFile << output.str() << endl;
    outFile.close();
    cout << "Tabla creada exitosamente." << endl;
  } else {
    cerr << "No se pudo abrir el archivo de esquemas." << endl;
  }
}

void Database::insertInSchema(string &command) {
  ifstream schemaFile("../../data/usr/db/schemas.txt");
  if (!schemaFile.is_open()) {
    cerr << "No se pudo abrir el archivo de esquemas." << endl;
    return;
  }

  size_t pos2 = command.find('(');
  string tableNameFromUser = command.substr(12, pos2 - 12);

  string lineNow, tableSchema;

  bool tableExists = false;
  while (getline(schemaFile, lineNow)) {
    if (lineNow.find(tableNameFromUser) == 0) {
      tableSchema = lineNow;
      tableExists = true;
      break;
    }
  }

  if (!tableExists) {
    cerr << "La tabla " << tableNameFromUser << " no existe" << endl;
    return;
  }
  tableSchema = tableSchema.substr(tableNameFromUser.size() + 1);
  /*
  cout << tableSchema << endl;
  cout << tableNameFromUser << endl;
  */

  fstream dataFile("../../data/usr/db/Estudiantes.txt");

  if (!dataFile.is_open()) {
    cerr << "Error for open Estudiantes.txt" << endl;
    return;
  }

  size_t initData = command.find('(');
  size_t endData = command.find(')');
  if (initData == string::npos || endData == string::npos || endData <= initData) {
    cerr << "Insertion fail please use the correct format (value, value, ...)" << endl;
    dataFile.close();
    return;
  }
}

void Database::showTables() {
}

void Database::selectTable(string &command) {
  size_t pos1 = command.find("select * from");

  if (pos1 == string::npos) {
    cerr << "No existe la tabla seleccionada" << endl;
    return;
  }

  size_t pos2 = command.find("from");
  if (pos2 == string::npos) {
    cerr << "Error: Debes selecciona ua tabla" << endl;
    return;
  }
}
