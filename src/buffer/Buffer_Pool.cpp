#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "buffer/BufferPool.h"
#include "buffer/Clock.h"
#include "buffer/Frame.h"

using namespace std;

BufferPool::BufferPool() {
  this->numFrames = 0;
  this->capacity = 720;
}

BufferPool::BufferPool(int numFrames) {
  this->numFrames = numFrames;
  this->capacity = 720;
  my_clock.setSize(numFrames);
  for (int i = 0; i < this->numFrames; i++) {
    Frame frame(i);
    this->frames.push_back(frame);
    page_table[i] = -1; // CON -1 VALOR PREDETERMINADO(VACIO) Y LA CLAVE ES EL
                        // FRAMEid Y EL VALOR EL PAGEid
  }
  FOR_LOOP(0, i, this->numFrames) { firstFree.push(i); }
}

vector<Frame> &BufferPool::getFrames() { return frames; }

Frame &BufferPool::getFrame(int frameID) {
  for (int i = 0; i < frames.size(); i++) {
    if (frames[i].getframeID() == frameID) {
      return frames[i];
    }
  }
  throw std::runtime_error("Frame not found");
}

void BufferPool::modifyPinInExistingFrame(int pageID, char flag) {
  for (int i = 0; i < frames.size(); i++) {
    if (page_table[frames[i].getframeID()] == pageID) {
      if (flag == 'i') {
        frames[i].incrementPinCount();
        frames[i].setRefBit(1);
      } else if (flag == 'k' && frames[i].getPinCount() > 0) {
        frames[i].decrementPinCount();
      }
    }
  }
}

void BufferPool::setPage(Frame &frame, int frameID) {
  if (frameID >= 0 && frameID < numFrames) {
    frames[frameID] = frame;
    page_table[frameID] = 1;
  } else {
    cout << "SetP: Frame ID fuera de rango" << endl;
    return;
  }
  cout << "\nPagina [" << frameID << "] cargada correctamente." << endl;
}

void BufferPool::addRecord(int frameID, string record) {
  if (frameID >= 0 && frameID < numFrames) {
    if (page_table[frameID] == 1) {
      Frame &frame = frames[frameID];
      // frame.getPage().addRecord(record);
      cout << "Registro agregado correctamente." << endl;
    } else {
      cout << "Frame ID: " << frameID << " no contiene una página." << endl;
    }
  } else {
    cout << "Frame ID fuera de rango." << endl;
  }
}

void BufferPool::deleteRecord(int frameID, string record) {
  if (frameID >= 0 && frameID < numFrames) {
    if (page_table[frameID] == 1) {
      Frame &frame = frames[frameID];
      // frame.getPage().deleteRecord(record);
      cout << "Registro eliminado correctamente." << endl;
    } else {
      cout << "Frame ID: " << frameID << " no contiene una página." << endl;
    }
  } else {
    cout << "Frame ID fuera de rango." << endl;
  }
}

void BufferPool::freeFrame(int frameID) {
  if (frameID >= 0 && frameID < numFrames) {
    frames[frameID] = Frame(frameID);
    page_table[frameID] = -1;
    cout << "Frame [" << frameID << "] liberado correctamente." << endl;
  } else {
    cout << "Frame ID fuera de rango." << endl;
  }
}

int BufferPool::findFreeFrame() {
  if (!firstFree.empty()) {
    int frameL = firstFree.front();
    firstFree.pop();
    return frameL;
  } else {
    for (auto it = page_table.begin(); it != page_table.end(); ++it) {
      if (it->second == -1)
        return it->first;
    }
  }
  /*Si retorna -2 es porque todos los frames ya tienen cargado una pagina*/
  return -2;
}

/*void BufferPool::setPageInFrame(int frameID, int pageID, Frame &frame) {
  if (frameID >= 0 && frameID < numFrames) {
    frames[frameID] = frame;
    page_table[frameID] = pageID;
  } else {
    cout << "SetP: Frame ID fuera de rango" << endl;
    return;
  }

  // printTableFrame();

  cout << "\nPagina [" << frameID << "] cargada Correctamente\n"
      << endl;
  setHistory(pageID);
}*/
// ERICK MALCOACCHA Y SEBASTIAN MENDOZA
void BufferPool::setPageInFrame2(int frameID, int pageID, bool dirty,
                                 Page page) {
  if (frameID >= 0 && frameID < numFrames) {
    page_table[frameID] = pageID;
    frames[frameID].setPage(page);
    frames[frameID].setDirtyFlag(dirty);
    frames[frameID].setPinCount(1);
    frames[frameID].setRefBit(1);
    frames[frameID].addRequest(dirty);

  } else {
    cout << "SetP: Frame ID fuera de rango" << endl;
    return;
  }

  // printTableFrame();

  cout << "\nPagina [" << frameID << "] cargada Correctamente\n" << endl;
  setHistory(pageID);
}

bool BufferPool::isPageLoaded(int pageID) {
  for (const auto &pair : page_table) {
    if (pair.second == pageID) {
      return true;
    }
  }
  return false;
}

int BufferPool::getFrameId(int pageID) {
  for (const auto &pair : page_table) {
    if (pair.second == pageID) {
      return pair.first;
    }
  }
  return -1;
}

// SERGIO CASTILLO
void BufferPool::printTableFrame() {
  cout << setw(10) << "Frame Id"
       << "\t" << setw(10) << "Page Id"
       << "\t" << setw(10) << "Dirty Bit"
       << "\t" << setw(10) << "Pin Count"
       << "\t" << setw(10) << "Ref Bit"
       << "\t" << setw(10) << "Pinned"
       << "\t\n";

  for (int i = 0; i < frames.size(); i++) {
    cout << setw(10) << frames[i].getframeID() << "\t" << setw(10)
         << frames[i].getPage().getPageId() << "\t" << setw(10)
         << (frames[i].isDirty() ? "Yes" : "No") << "\t" << setw(10)
         << frames[i].getPinCount() << "\t" << setw(10) << frames[i].getRefBit()
         << setw(10) << (frames[i].getPinned() ? "Yes" : "No") << "\t"
         << setw(10) << "\t\n";
  }
}

void BufferPool::setHistory(int pageID) { history[pageID] = 0; }

void BufferPool::incrementHistory() {
  for (auto &pair : page_table) {
    if (pair.second != -1)
      history[pair.first]++;
  }
}

void BufferPool::LRU() {
  cout << "LRU" << endl;
  /*
  En el unordered_map history, se guardará el frameID y el tiempo en el que fue
  accedido. Atraves de las veces que se llame en general con +1. Ahora se
  buscara cual frame tiene mas tiempo en estar. Luego se revisara si el pin
  count esta en cero y ahi recien se llamara la funcion freeFrame.
  */
  int max = 0;
  int lastUsedFrame = 0;
  for (auto &pair : history) {
    if (pair.second > max) {
      max = pair.second;
      lastUsedFrame = pair.first;
    }
  }
  if (frames[lastUsedFrame].getPinCount() == 0) {
    history[lastUsedFrame] = 0;
    freeFrame(lastUsedFrame);
  } else {
    cout << "No se puede liberar el frame " << lastUsedFrame
         << " porque tiene un pin count mayor a 0" << endl;
  }
}

// FUNCION DE POLITICA DE REEMPLAZO
// ERIK RAMOS QUISPE
/*void BufferPool :: clockPolicy(int pagID, string path, bool mode){
  bool cero = false;
  int posFrame = -1;
  bool liberarP = true;
  int vuelta = 0;
  while(!cero){ //ITERAR HASTA ENCONTRAR UN REFBIT EN 0

    FOR_LOOP(my_clock.getHandClock(), i, numFrames){ //BUSCARA UN 0 A PARTIR DEL
HANDCLOCK if(frames[i].getRefBit() != -1){//VERIFICARA SI AL FRAME SE LE ASIGNO
UNA PAGINA if(frames[i].getPinCount() == 0 && frames[i].getRefBit() == 1){ //SI
EL GETREFBIT ESTA EN 1 LO CAMBIA A 0 my_clock.incrementHC();
          if(my_clock.getHandClock() > (numFrames - 1)){
            my_clock.setHandClock(0);
          }
          cout << "Valor de handClock -> " <<  my_clock.getHandClock() << "\n";
          frames[i].setRefBit(0);
          liberarP = false;
        }
        else if(frames[i].getPinCount() == 0 && frames[i].getRefBit() == 0){
//DETENER EL BUCLE FOR SI ENCONTRAMOS UN GETREFBIT 0 my_clock.incrementHC();
          if(my_clock.getHandClock() > (numFrames - 1)){
            my_clock.setHandClock(0);
          }
          cero = true; //DETENER EL BUCLE WHILE, CONSEGUIMOS UN 0 EN GETREFBIT
          posFrame = frames[i].getframeID(); // GUARDAR LA POSICION DEL FRAME
QUE SU GETREFBIT ES 0 PARA CAMBIARLO; liberarP = false; break;
        }
        else{
          my_clock.incrementHC();
          if(my_clock.getHandClock() > (numFrames - 1)){
            my_clock.setHandClock(0);
          }
          cout << "Valor de handClock, pincount > 0 -> " <<
my_clock.getHandClock() << "\n";
        }
        vuelta++;//CONTAR PARA SIMULAR SI EN TODA LA VUELTA EL PINCOUNT ES MAYOR
A 0 EN TODOS LOS FRAMES
      }
    }
    if(vuelta == numFrames && liberarP){//SI SE DIO TODA LA VUELTA SIN PINCOUNT
= 0 SE ROMPE EL BUCLE WHILE break;
    }
  }
  if(!liberarP){//ACTUALIZA LA INFORMACION CON LA NUEVA PAGINA
    freeFrame(posFrame);
    int frameFree = findFreeFrame();
    Page cambiarPage;
    cambiarPage.setPageId(pagID);
    cambiarPage.setName(path);
    frames[frameFree].setPage(cambiarPage);
    frames[frameFree].setDirtyFlag(mode);
    frames[frameFree].setPinCount(1);
    frames[frameFree].setRefBit(1);
    page_table[frameFree] = pagID;
    cout << "Valor de handClock encontro un 0(manecilla avanzo)-> " <<
my_clock.getHandClock() << "\n";
  }
  else{//SI SE DA TODA LA VUELTA SIN UN PINCOUNT = 0
    cout << "TIENES QUE LIBERAR PROCESOS, TODOS LAS PAGINAS ESTAN SIENDO
UTILZADAS\n";
  }
}*/

int BufferPool ::findRefBit0(int &iteratorTwoTurns) {
  bool rfBit = true;
  int posFrame = 0;
  while (rfBit) {
    for (int i = my_clock.getHandClock(); i < numFrames; i++) {
      if (frames[i].getPinned()) {
        iteratorTwoTurns++;
        my_clock.incrementHC();
        continue;
      }

      if (frames[i].getRefBit() == 0) {
        iteratorTwoTurns++;
        my_clock.incrementHC();
        posFrame = i;
        rfBit = false;
        break;
      } else {
        frames[i].setRefBit(0);
      }

      iteratorTwoTurns++;
      my_clock.incrementHC();
    }
  }
  return posFrame;
}

void BufferPool::findMinPinCount(int &posFrame) {
  int menor_pc = frames[posFrame].getPinCount();
  bool vuelta = false;
  int contar = 0;
  while (!vuelta) {
    for (int i = my_clock.getHandClock(); i < numFrames; i++) {
      i++;
      if (contar == numFrames) {
        vuelta = true;
        break;
      }
      int posNewFrame = findRefBit0(contar);
      if (frames[posNewFrame].getPinCount() < menor_pc) {
        posFrame = posNewFrame;
      }
    }
  }
}

int BufferPool ::clockPolicy() {
  // necesitamos un dirty = 0 y un refbit = 0;
  // sino dirty = 1, refbit = 0;
  int iteratorTwoTurns = 0;
  int posFrame = findRefBit0(iteratorTwoTurns);
  while (iteratorTwoTurns <= 2 * numFrames) {
    if (!frames[posFrame].isDirty()) {
      break;
    } else {
      posFrame = findRefBit0(iteratorTwoTurns);
    }
  }
  findMinPinCount(posFrame);
  return posFrame;
}

int BufferPool ::clock_Replacement(int pageID, string path, bool mode, string content, int capacity, string header) {
  int posFrame = clockPolicy();
  if (frames[posFrame].getPinCount() > 0) {
    cout << "==============================================\n";
    cout << "Tienes que liberar procesos, todas las paginas estan siendo "
            "utilizadas\n";
    int pageID = frames[posFrame].getPage().getPageId();
    cout << "Clock Necesita liberar la pagina: "
         << pageID << endl;
    cout << "==============================================\n";
    my_clock.decrementHC();
    return pageID;
  } else {
    freeFrame(posFrame);
    int frameFree = findFreeFrame();
    Page cambiarPage;
    cambiarPage.setPageId(pageID);
    cambiarPage.setName(path);
    cambiarPage.setSize(capacity);
    cambiarPage.setContentRFL(content);
    cambiarPage.setHeaderRFL(header);
    frames[frameFree].setPage(cambiarPage);
    frames[frameFree].setDirtyFlag(mode);
    frames[frameFree].setPinCount(1);
    frames[frameFree].setRefBit(1);
    frames[frameFree].addRequest(mode);
    page_table[frameFree] = pageID;
    cout << "==============================================\n";
    cout << "Clock Reemplazo la pagina: "
         << frames[frameFree].getPage().getPageId() << endl;
    cout << "==============================================\n";
  }
  return -1;
}

string BufferPool ::getPathPage(int pageID) {
  for (int i = 0; i < frames.size(); i++) {
    if (frames[i].getPage().getPageId() == pageID) {
      return frames[i].getPage().getName();
    }
  }
  return "No se encontro la pagina";
}