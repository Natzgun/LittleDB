#ifndef QUERY_H
#define QUERY_H
#include <iostream>
#include <vector>
using namespace std;

class Query {
private:
  
public:
  Query();
  /*static void selectAllColumns(string &tableName);
  static void selectColumn(string &tableName, int columnIndex);
  static void selectWithCondition(string &tableName, string &condition, int colIndex);
  static bool compare(int value, string &condition);*/

  static void optionsSearchs(int option, int desplazamiento, string records,int inicio, int length, int* current, string key);
  static int menuOptions();
  static void selectAllColumns(string &tableName);
  static void searchKey(string key, string records, int desplazamiento, pair<vector<string>,pair<int,int>> colAndPos);
  static int selectOptionRange();
  static pair<int,int> positionOfSearch(pair<int,int> longest);
  static int selectColumna(vector<string> columns);
  static void selectForRange(string key, string records, int desplazamientom, pair<vector<string>,pair<int,int>> colAndPos);
};
#endif //QUERY_H
