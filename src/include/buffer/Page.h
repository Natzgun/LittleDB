#ifndef PAGE_H
#define PAGE_H

#include <string>
#include <vector>

using namespace std;

class Page {
private:
  int capacity;
  string namePath;
  int pageID;
  string contentRFL; // contenido registro de longitud fija

public:
  Page();

  Page(int Size, string Name);

  int getSize();

  void setSize(int size);

  int getPageId();

  void setPageId(int pageId);

  string getName();

  void setName(string name);

  // Añade el contenido de un registro a la página si hay suficiente espacio
  bool addRecordInContent(string &record);

  // Elimina un registro de la página en el índice especificado
  bool deleteRecordInContent(int index);

  // Devuelve el contenido de todos los registros de la página
  string &getContent();

  void setContentRFL(string content);
};

#endif  // PAGE_H
