#include "console/Console.h"
#include <iostream>
#include <limits>
#include <vector>

using namespace std;


Console::Console() {
  storage = new Storage();
}

void Console::run() {
  int menu{0};
  for (;;) {
    cout << "\tDATABASE" << endl;
    cout << "\t(1) New table " << endl;
    cout << "\t(2) Show table " << endl;
    cout << "\t(5) Salir " << endl;
    cout << "Opcion : ";
    cin >> menu;
    cout << "\n ---> SEGUNDA OPCION\n";
    switch (menu) {
      case 1:
        createTable();
        break;
      case 2:
        cout << "Gracias por usar el Sistema" << endl;
        break;
      default:
        cout << "Bye\n";
        return;
    }
  }
}

void Console::createTable() {
  vector<string> data;
  string word;

  cout << "Table name: ";
  cin >> word;
  data.push_back(word);
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  while (true) {
    getline(cin, word);
    if (word.empty())
      break;
    data.push_back(word);
    word = "";
  }
  cout << "Palabras ingresadas:" << endl;
  for (const string &w: data) {
    cout << w << endl;
  }

  storage->initSchema(data);
}
