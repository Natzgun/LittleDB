#include "database/Query.h"

int Query::menuOptions(){ 
  cout << "1. Select all columns" << endl;
  cout << "2. Search for a key" << endl;
  cout << "3. Select for a range" << endl;
  cout << "4. Exit" << endl;
  cout << "Enter your choice: ";
  int choice; cin >> choice;
  return choice;
}

void Query::selectAllColumns(string &tableName){
  cout << "SELECT * FROM " << tableName << endl;

}

void Query::searchKey(string key, string records, int desplazamiento, pair<vector<string>,pair<int,int>> colAndPos){
  int setOptionRange = selectColumna(colAndPos.first);
  int condition = selectOptionRange();
  int inicio = colAndPos.second.first;
  int fin = colAndPos.second.second;
  int length = fin - inicio + 1; 
  int current = desplazamiento;
  int* pointerCurrent = &current;
  optionsSearchs(1,desplazamiento,records,inicio,length,pointerCurrent,key);
}



int Query:: selectColumna(vector<string> columns){
  cout << "Select the column to search: " << endl;
  for(int i = 0; i < columns.size(); i++){
    cout << i+1 << ". " << columns[i] << endl;
  }
  int column; cin >> column;
  return column;
}

int Query::selectOptionRange(){
  cout << "Enter the condition: ";
  cout << "1. >=" << endl;
  cout << "2. <=" << endl;
  int condition; cin >> condition;
  return condition;
}


void Query::selectForRange(string key, string records, int desplazamiento, pair<vector<string>,pair<int,int>> colAndPos){
  int setOptionRange = selectColumna(colAndPos.first);
  int condition = selectOptionRange();
  int inicio = colAndPos.second.first;
  int fin = colAndPos.second.second;
  int length = fin - inicio + 1; 
  int current = desplazamiento;
  int* pointerCurrent = &current;
  if(condition == 1){
    cout << "SELECT * FROM " << records << " WHERE " << colAndPos.first[setOptionRange - 1] << " <= " << key << endl;
    optionsSearchs(2,desplazamiento,records,inicio,length,pointerCurrent,key);

  }else{
    cout << "SELECT * FROM " << records << " WHERE " << colAndPos.first[setOptionRange - 1] << " >= " << key << endl;
    optionsSearchs(3,desplazamiento,records,inicio,length,pointerCurrent,key); 
  }
}

void Query::optionsSearchs(int option, int desplazamiento, string records,int inicio, int length, int* current, string key){
  for(int i = 0; i < records.size(); i+= desplazamiento){
    string recordCut = records.substr(*current - 1,desplazamiento); 
    string keyCurrent = recordCut.substr(inicio,length);
    *current += desplazamiento;
    if(option == 1){
      if(key == keyCurrent){
        cout << recordCut << endl;
      }
    }else if(option == 2){
      if(key >= keyCurrent){
        cout << recordCut << endl;
      }
    }
    else if(option == 3){
      if(key <= keyCurrent){
        cout << recordCut << endl;
      }
    }
    else{
      cout << "ERROR NO HAY ESTA OPCION" << endl;
    }
  }
}


