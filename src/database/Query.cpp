#include "database/Query.h"

int Query::menuOptions(){ 
  cout << "1. Select all columns" << endl;
  cout << "2. Search for a key" << endl;
  cout << "3. Select for a range key <= X " << endl;
  cout << "4. Select for a range key >= X " << endl;
  cout << "5. Exit" << endl;
  cout << "Enter your choice: ";
  int choice; cin >> choice;
  return choice;
}

void Query::selectAllColumns(string records, int desplazamiento, pair<int,int> colAndPos){
  int inicio = colAndPos.first;
  int fin = colAndPos.second;
  int length = fin - inicio + 1; 
  int current = 0;
  int* pointerCurrent = &current;
  optionsSearchs(1,desplazamiento,records,inicio,length,pointerCurrent);
  
}

void Query::searchKey(string key, string records, int desplazamiento, pair<int,int> colAndPos){
  int inicio = colAndPos.first;
  int fin = colAndPos.second;
  int length = fin - inicio + 1; 
  int current = 0;
  int* pointerCurrent = &current;
  optionsSearchs(2,desplazamiento,records,inicio,length,pointerCurrent,key);
}



int Query:: selectColumna(vector<string> columns){
  cout << "Select the column to search: " << endl;
  for(int i = 0; i < columns.size(); i++){
    cout << i+1 << ". " << columns[i] << endl;
  }
  int column; cin >> column;
  return column;
}


void Query::selectForRangeMayor(string key, string records, int desplazamiento, pair<vector<string>,pair<int,int>> colAndPos){
  static int condition;
  int inicio = colAndPos.second.first;
  int fin = colAndPos.second.second;
  int length = fin - inicio + 1; 
  int current = 0;
  int* pointerCurrent = &current;
  optionsSearchs(3,desplazamiento,records,inicio,length,pointerCurrent,key);
}

void Query::selectForRangeMenor(string key, string records, int desplazamiento, pair<vector<string>,pair<int,int>> colAndPos){
  static int condition;
  int inicio = colAndPos.second.first;
  int fin = colAndPos.second.second;
  int length = fin - inicio + 1; 
  int current = 0;
  int* pointerCurrent = &current;
  optionsSearchs(4,desplazamiento,records,inicio,length,pointerCurrent,key);
}

string Query::removeSpaces(string input) {
    std::string result;
    for (char c : input) {
        if (c != '\n') {
            result += c;
        }
    }
    return result;
}
void Query::optionsSearchs(int option, int desplazamiento, string records,int inicio, int length, int* current, string key){
  string recordsSinNewLine = removeSpaces(records);
  int longitud = recordsSinNewLine.size();
  for(int i = 0; i < longitud; i+= desplazamiento){
    string recordCut = recordsSinNewLine.substr(*current,desplazamiento); 
    string keyCurrent = recordCut.substr(inicio,length);
    *current += desplazamiento + 1;
    if(option == 1){
      cout << recordCut << endl;
    }else if(option == 2){
      if(key == keyCurrent){
        cout << recordCut << endl;
      }
    }
    else if(option == 3){
      if(key <= keyCurrent){
        cout << recordCut << endl;
      }
    }
    else if(option == 4){
      if(key >= keyCurrent){
        cout << recordCut << endl;
      }
    }
    else{
      cout << "ERROR NO HAY ESTA OPCION" << endl;
    }
  }
}


