
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
  static void selectWithCondition(const string &tableName, const string &condition);

};
#endif //QUERY_H
