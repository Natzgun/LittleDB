#include "console/Console.h"
#include <iostream>
#include <limits>

using namespace std;


Console::Console(Database &db) : db(db) {
};

void Console::run() {
  string command;

  cout << "Welcome BD alpha project MEGATRON" << endl;

  while (true) {
    cout << "[LittleDB]host >> ";
    getline(cin, command);

    execCommand(command);

    if (command == "exit") {
      cout << "Gracias por usar el Sistema." << endl;
      break;
    }
  }
}

void Console::parseSelect(string &selectCommand) {
  int posFrom = selectCommand.find("from");
  if (posFrom == string::npos) {
    cerr << "Error: Debes seleccionar una tabla" << endl;
    return;
  }

  int posWhere = selectCommand.find("where");
  int endTable = selectCommand.find(';');

  if (endTable == string::npos) {
    cerr << "Error: La consulta debe terminar con ';'" << endl;
    return;
  }

  if (posWhere != string::npos && posWhere < endTable) {
    this->tableName = selectCommand.substr(posFrom + 5, posWhere - posFrom - 6);
    this->columns = selectCommand.substr(7, posFrom - 8);
    this->condition = selectCommand.substr(posWhere + 6, endTable - posWhere - 6);
  } else {
    this->tableName = selectCommand.substr(posFrom + 5, endTable - posFrom - 5);
    this->columns = selectCommand.substr(7, posFrom - 8);
    this->condition = "";
  }

  cout << "Table Name: " << tableName << endl;
  cout << "Columns: " << columns << endl;
  cout << "Condition: " << condition << endl;
  cout << "-------------------" << endl;
}

void Console::cleanCommands() {
  this->tableName = "";
  this->columns = "";
  this->condition = "";
}

void Console::execCommand(string &command) {
  if (command.find("create table") == 0) {
    db.createTable(command);
  } else if (command == "show tables") {
    db.showTables();
  }else if (command.find("insert into") == 0) {
    db.insertInSchema(command);
  } else if (command.find("select") == 0){
    parseSelect(command);
    db.selectTable(tableName, columns, condition);
    cleanCommands();
  } else if (command.find("ReadCSV") == 0){
    db.readCSV(command);
  } else {
    cout << "Comando no reconocido." << endl;
  }
}
