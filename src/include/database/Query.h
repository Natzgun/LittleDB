
#ifndef QUERY_H
#define QUERY_H
#include <iostream>
using namespace std;

class Query {
private:
public:
  Query();
  static void selectAllColumns(string &tableName);
  static void selectColumn(string &tableName, int columnIndex);

};
#endif //QUERY_H
