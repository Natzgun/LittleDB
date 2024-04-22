#include <iostream>
#include "console/Console.h"
#include "database/Database.h"

int main() {
  Database db;
  Console shell(db);
  shell.run();
  return 0;
}
