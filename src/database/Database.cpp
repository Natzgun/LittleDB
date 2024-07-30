#include "database/Database.h"
#include "database/Query.h"

#include <algorithm>
#include <fstream>
#include <sstream>

string Database::schemaExists(string &tableName) {
  ifstream schemaFile("../../data/usr/db/schemas.txt");
  if (!schemaFile.is_open()) {
    cerr << "Error: No se pudo abrir el archivo de esquemas." << endl;
    return "notFound";
  }

  string tableSchema;
  string line;
  bool foundTable = false;
  while (getline(schemaFile, line)) {
    if (line.find(tableName) == 0) {
      tableSchema = line;
      foundTable = true;
      break;
    }
  }
  schemaFile.close();

  if (!foundTable) {
    cerr << "Error: No se encontró la definición de la tabla '" << tableName << "' en el archivo de esquemas." << endl;
    return "notFound";
  } else {
    return tableSchema;
  }
}

string Database::separator(string &lineToSeparate) {
  /*
  short cont = 0;
  while (lineToSeparate)
  */
  return "Todavia falto";
}

int Database::getColumnIndex(string &tableName, string &columns) {
  string schemaExtracted = schemaExists(tableName);
  schemaExtracted = schemaExtracted.substr(tableName.size() + 1);
  int columnIndex = validator.obtainColumnPosition(schemaExtracted, columns);
  if (columnIndex == -1) {
    cerr << "Error: La columna '" << columns << "' no se encontró en el esquema." << endl;
  }

  return columnIndex;
}

int Database::extractFixedLenghtRecord(string &relation) {
  stringstream ss(relation);
  string part;
  int sum = 0;

  while (getline(ss, part, '#')) {
    if (isdigit(part[0])) {
      sum += stoi(part);
    }
  }

  return sum;
}

string Database::formatRecordRLF(string &relationSchema, string &record) {
  stringstream ssRelation(relationSchema);
  stringstream ssRecord(record);
  string partRelation, partRecord;
  string result = "";

  while (getline(ssRelation, partRelation, '#')) {
    string dataType;
    int length;

    // Leer el tipo de dato y el tamaño del campo
    getline(ssRelation, dataType, '#');
    getline(ssRelation, partRelation, '#');
    length = stoi(partRelation);

    partRecord = "";

    if (ssRecord.peek() == '"') {
      ssRecord.get(); // Descartamos la primera comilla
      while (true) {
        string temp;
        getline(ssRecord, temp, '"'); // Leemos hasta la siguiente comilla
        partRecord += temp;
        if (ssRecord.peek() == ',') {
          ssRecord.get(); // Descartamos la coma después de la comilla de cierre
          break;
        } else {
          char nextChar = ssRecord.get();
          if (nextChar == EOF) {
            break;
          }
          partRecord += nextChar; // Añadir la comilla que falta ya que no hubo ninguna coma
        }
      }
    } else {
      getline(ssRecord, partRecord, ',');
    }

    partRecord.resize(length, ' ');
    result += partRecord;
  }

  return result;
}

string Database::formatRecordRLV(string &relationSchema, string &record) {
  /*
  string variablePairPosition = "";
  string fixedRecord = "";
  string bitsMap = "";
  string variableRecord = "";

  return variableRecord + "|" + fixedRecord + "|" + bitsMap + "|" + variablePairPosition;
  */

  string variablePairPosition = "";
  string fixedRecord = "";
  string bitsMap = "";
  string variableRecord = "";

  istringstream schemaStream(relationSchema);
  istringstream recordStream(record);

  string schemaToken, recordToken;

  int currentPos = 0; // Inicializamos currentPos en 0
  int variableStartPos = 0;
  int columnCount = 0;

  while (getline(schemaStream, schemaToken, '#')) {
    string columnName = schemaToken;
    getline(schemaStream, schemaToken, '#');
    string columnType = schemaToken;
    getline(schemaStream, schemaToken, '#');
    string columnSize = schemaToken;

    getline(recordStream, recordToken, ',');

    if (columnSize == "v") {
      variablePairPosition += to_string(currentPos) + "," + to_string(recordToken.size()) + "|";
      variableRecord += recordToken;
      currentPos += recordToken.size(); // Actualizamos la posición actual
    } else {
      int size = stoi(columnSize);
      if (columnType == "entero" || columnType == "string") {
        fixedRecord += recordToken;
        fixedRecord.append(size - recordToken.size(), ' ');
        currentPos += size; // Actualizamos la posición actual
      }
    }
    columnCount++;
  }

  // Elimina el último '|' de variablePairPosition si no está vacío
  if (!variablePairPosition.empty()) {
    variablePairPosition.pop_back();
  }

  // Generar el mapa de bits con tantos ceros como columnas haya
  bitsMap = string(columnCount, '0');

  return variablePairPosition + "|" + fixedRecord + "|" + bitsMap + "|" + variableRecord;
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

  // cout << tableSchema << endl;
  // cout << tableNameFromUser << endl;


  /*
  ofstream dataFile("../../data/root_directory/platter1/track1/sector1.txt", ios::app);

  if (!dataFile.is_open()) {
    cerr << "Error al abrir .txt" << endl;
    return;
  }
  */

  size_t initData = command.find('(');
  size_t endData = command.find(')');
  if (initData == string::npos || endData == string::npos || endData <= initData) {
    cerr << "Inserta de forma correcta (value, value, ...)" << endl;
    return;
  }

  // Lo anterior era para extraer los datos del comando
  string values = command.substr(initData + 1, endData - initData - 1);

  /* Se extrae el tamaño del registro se le da su formato adecuado segun la longitud
   * de cada registro y se inserta en el archivo de datos correspondiente
   */
  int fixedLength = extractFixedLenghtRecord(tableSchema);
  //values = formatRecordRLF(tableSchema, values);
  char mode;
  cout << "Variable o Fija: ";
  cin >> mode;
  if (mode == 'V') {
    values = formatRecordRLV(tableSchema, values);
  } else {
    values = formatRecordRLF(tableSchema, values);
  }
  //values = formatRecordRLF(tableSchema, values);
  dbMediator.addRecord(tableNameFromUser, values);
  //dataFile << values << endl;

  schemaFile.close();
}


void Database::selectTable(string &tableName, string &columns, string &condition) {
  if (schemaExists(tableName) == "notFound")
    return;

  if (columns == "*") {
    // Query::selectAllColumns(tableName);
    if (condition.empty()) {
      Query::selectAllColumns(tableName);
      cout << "Seleccionando todo" << endl;
    } else {
      string colNameofCondition = condition.substr(0, condition.find(' '));
      int colIndex = getColumnIndex(tableName, colNameofCondition);
      string restCondition = condition.substr(condition.find(' ') + 1);
      Query::selectWithCondition(tableName, restCondition, colIndex);
    }
  } else {
    Query::selectColumn(tableName, getColumnIndex(tableName, columns));
  }

  cout << "Select de la tabla '" << tableName << "'." << endl;
}

void Database::readCSV(string &command) {
  string tablename = command.substr(8);

  if (schemaExists(tablename) == "notFound") {
    return;
  }

  string tableSchema = schemaExists(tablename);
  tableSchema = tableSchema.substr(tablename.size() + 1);

  ifstream csvFile("../../data/usr/db/" + tablename + ".csv");
  if (!csvFile.is_open()) {
    cerr << "Error: No se pudo abrir el archivo CSV '" << tablename << ".csv'." << endl;
    return;
  }

  /* Estas  son las funciones que se encargan de de leer el CSV calcula su tamaño
   * los formatea y luego los inserta de forma consecutiva a su bloque correspondiente
   */
  string lineOfCSV;
  int fixedLength = extractFixedLenghtRecord(tableSchema);
  while (getline(csvFile, lineOfCSV)) {
    if (lineOfCSV.back() == '\r')
      lineOfCSV.pop_back();

    string values = formatRecordRLF(tableSchema, lineOfCSV);
    dbMediator.addRecord(tablename, values, true);
  }
  dbMediator.addRecord(tablename,"1000",false,true);

  csvFile.close();

  cout << "Datos del archivo CSV '" << tablename << ".csv' insertados en la tabla '" << tablename << ".txt'." << endl;
}

void Database::selectDisk(bool defaultDisk) {
  dbMediator.selectDiskStructureMediator(defaultDisk);
}

void Database::viewRam() {
  dbMediator.adminRam();
}

void Database::saveDataInRAM() {
  dbMediator.saveDataInRAM();
}

void Database::loadDataInFiles() {
  dbMediator.loadDataInFiles();
}

void Database::adminBtree() {
  dbMediator.adminBplusTree();
}
