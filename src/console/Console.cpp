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

void Console::execCommand(string &command) {
  if (command.find("create table") == 0) {
    db.createTable(command);
  } else if (command == "show tables") {
    db.showTables();
  }else if (command.find("insert into") == 0) {
    db.insertInSchema(command);
  } else if (command.find("select") == 0){
    db.selectTable(command);
  } else if (command.find("ReadCSV") == 0){
    db.readCSV(command);
  } else {
    cout << "Comando no reconocido." << endl;
  }
}
