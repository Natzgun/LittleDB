#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include "Frame.h"
#include "Clock.h"

/*Sebastian Andres */

using namespace std;

class BufferPool {
private:
  int numFrames;
  int capacity;
  clockP my_clock;
  vector<Frame> frames;
  unordered_map<int, int> history;
  unordered_map<int, int> page_table;
  queue<int> firstFree;

public:
  BufferPool(); /* Constructor por defecto */
  BufferPool(int numFrames); /* Constructor donde ira creando los frames vacios */
  vector<Frame> &getFrames(); /* Sera para obtener el vector que contiene los frames */
  Frame &getFrame(int frameID); /* Obtener un frame en especifico */
  void modifyPinInExistingFrame(int pageID, char flag); /* Incrementar el pin de un frame */
  void setPage(Frame &frame, int frameID);

  /* Cargar una pagina en un frame usando un frameID que nosostros le daremos */
  void printPage(int frameID); /* Mostrar el contenido de la pagina, pero hay que comprobar que este el frameid */
  void addRecord(int frameID, string record); /* Usando la ubicacion del frame la agregamos un registro */
  void deleteRecord(int frameID, string record); /* Lo anterio pero para eliminar */
  void pageIsDirty(int pageID); /* le decimos al frame que sea diry o comprobar para cambiar a void*/
  void freeFrame(int frameID); /* Liberar un frame */
  int findFreeFrame(); /* Encontrar un frame libre */
  void setPageInFrame(int frameID, int pageID, Frame &page); /* Cargar una pagina en un frame */
  bool isPageLoaded(int pageID); /* Comprobar si la pagina esta cargada */
  int getFrameId(int pageID); /* Obtener el frameID de una pagina */
  void printTableFrame(); /*Imprimimos todos los frames*/
  void setHistory(int pageID); /* Guardar el historial de las paginas */
  void incrementHistory(); /* Incrementar el historial de las paginas */
  void LRU(); /* El LRU */
  void setPinCount(int);
  void setPageInFrame2(int frameID, int pageID, bool dirty, Page page);
  int findRefBit0(int& iteratorTwoTurns);
  void findMinPinCount(int &posFrame);
  int clockPolicy();
  void clock_Replacement(int pageID, string path, bool mode);
};
