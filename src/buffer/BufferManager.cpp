#include "buffer/BufferManager.h"

#include <fstream>
#include <limits>
#include <sstream>
#include <string>

#include "buffer/Frame.h"
#include "buffer/Page.h"

// configura LRU (Least Recently Used) con el número total de frames.

BufferManager::BufferManager(int numFrames) {
  this->numFrames = numFrames;
  this->bpool = BufferPool(numFrames);
}

/*

Carga una página desde el disco al buffer pool.
Si todos los frames están ocupados, selecciona un frame para reemplazar usando
el algoritmo LRU guarda la página reemplazada en el disco y luego carga la nueva
página en ese frame.
*/

/*void BufferManager::loadPageFromDisk(string blockPath, int pageID, char _mode)
{ bool mode = _mode == 'W' ? true : false; if (bpool.isPageLoaded(pageID)){ cout
<< "La pagina ya esta cargada" << "\n"; if (_mode == 'L') {
      bpool.getFrame(bpool.getFrameId(pageID)).setDirtyFlag(false);
    } else {
      bpool.getFrame(bpool.getFrameId(pageID)).setDirtyFlag(true);
    }
    bpool.modifyPinInExistingFrame(pageID, 'i');
    bpool.incrementHistory();
    bpool.printTableFrame();
    return;
  }

  bpool.LRU();

  Page tempPage;
  tempPage.setName(blockPath);
  tempPage.setPageId(pageID);

  int idFree = bpool.findFreeFrame();
  Frame tempFrame(idFree);
  tempFrame.setDirtyFlag(mode);
  tempFrame.setPage(tempPage);
  tempFrame.incrementPinCount();

  bpool.setPageInFrame(idFree, pageID, tempFrame);
  bpool.incrementHistory();
  bpool.printTableFrame();
}*/

// ESTO CARGA UN BLOQUE DEL DISCO AL BUFFER MANAGER
// SEGIO CASTILLO Y ERICK MALCOACCHA
void BufferManager::loadPageFromDiskClock(int pageID, string path, char _mode) {
  bool mode = _mode == 'W' ? true : false;
  if (bpool.isPageLoaded(pageID)) { 
    bpool.getFrame(bpool.getFrameId(pageID)).addRequest(mode);
    cout << "La pagina ya esta cargada\n";
    if (_mode == 'L' && !bpool.getFrame(bpool.getFrameId(pageID)).isDirty()) {
      bpool.getFrame(bpool.getFrameId(pageID)).setDirtyFlag(false);
    } else {
      bpool.getFrame(bpool.getFrameId(pageID)).setDirtyFlag(true);
    }
    bpool.modifyPinInExistingFrame(pageID, 'i');
    bpool.printTableFrame();
    return;
  }

  // Open the file
  int blockCapacity = 0;
  std::string content;
  std::ifstream file(path);
  string header;
  if (file.is_open()) {
    std::string line;
    std::string target = "blockCapacity#";
    size_t pos;
    int lineCount = 0;

    /*
    while (std::getline(file, line)) {
      // Also add header

     if (lineCount < 5) {
       header += line + "\n";
       lineCount++;
     }

      pos = line.find(target);
      if (pos != std::string::npos) {
        blockCapacity = std::stoi(line.substr(pos + target.length()));
        break;
      }
    }
    */

    for (int i = 0; i < 5; i++) {
      getline(file, line);
      header += line + "\n";
      pos = line.find(target);
      if (pos != std::string::npos) {
        blockCapacity = std::stoi(line.substr(pos + target.length()));
      }
    }

    file.close();

    if (blockCapacity != 0) {
      std::cout << "El blockCapacity es: " << blockCapacity << std::endl;
    } else {
      std::cout << "No se encontró blockCapacity en el archivo." << std::endl;
    }
  } else {
    std::cout << "Error al cargar el archivo." << std::endl;
  }

  int valueF = bpool.findFreeFrame();
  if (valueF != -2) {
    Page tempPage;
    tempPage.setName(path);
    tempPage.setPageId(pageID);
    tempPage.setSize(blockCapacity);
    tempPage.setContentRFL(content);
    tempPage.setHeaderRFL(header);

    bpool.setPageInFrame2(valueF, pageID, mode, tempPage);
    bpool.printTableFrame();
    return;
  }
  cout << "\n****************************************************\n";
  int killPage = bpool.clock_Replacement(pageID, path, mode, content, blockCapacity, header);
  if(killPage != -1){
    killProcess(killPage); 
    bpool.clock_Replacement(pageID, path, mode, content, blockCapacity, header);
  }
  cout << "\n****************************************************\n";
  bpool.printTableFrame();
}

bool BufferManager::killProcess(int pageID) {
  /*
  if (bpool.getFrames()[bpool.getFrameId(pageID)].getPinned() == true) {
    cout << "Hey, hey, hey... Esta pagina esta PINEADA no la puedes tocar \n";
    return;
  }
  */
  bool ans = false;
  std::string killed;
  bpool.getFrames()[bpool.getFrameId(pageID)].showVector();
  int salida = bpool.getFrames()[bpool.getFrameId(pageID)].freeRequest();
  if (salida == 1) {
    if (bpool.getFrames()[bpool.getFrameId(pageID)].posLastWrite() == -1) {
      bpool.getFrames()[bpool.getFrameId(pageID)].setDirtyFlag(0);
    }
    cout << "Desea guardarlo en el disco?" << std::endl;
    cout << "Si/No: ";
    cin >> killed;
    if (killed == "Si") {
      savePageToDisk(pageID);
      ans = 1;
    }
  } else if (salida == -1) {
    cout << "No se puede liberar procesos que no existen\n";
  }
  bpool.modifyPinInExistingFrame(pageID, 'k');
  // bpool.incrementHistory();
  bpool.printTableFrame();
  return ans;
}

void BufferManager::printTableFrame() { bpool.printTableFrame(); }

void BufferManager::savePageToDisk(int pageID) {
  if (!bpool.isPageLoaded(pageID)) {
    cout << "La página con ID " << pageID << " no está cargada en el BufferPool.\n";
    return;
  }

  Page& page = bpool.getFrame(bpool.getFrameId(pageID)).getPage();

  string path = page.getName();
  string content = page.getContent();

  string header = page.getHeaderRFL();
  int sizePageNormal = page.getSize();

  size_t pos = header.find("blockCapacity#");
  if (pos != string::npos) {
    size_t endLine = header.find("\n", pos);
    header.replace(pos, endLine - pos, "blockCapacity#" + to_string(sizePageNormal));
  }

  ofstream file(path);
  if (file.is_open()) {
    file << header << endl;
    file << content << endl;
  } else {
    cout << "No se pudo abrir el archivo para guardar la pagina" << endl;
    file.close();
    return;
  }
  file.close();

  cout << "=============================================\n";
  cout << "Pagina guardado en disco :D" << endl;
  cout << "=============================================\n";
}

void BufferManager::updatePinnedPage() {
  cout << "(1) Pinear pagina" << endl;
  cout << "(2) Despinear pagina" << endl;
  char option;
  int idPinned;
  cin >> option;
  cout << "Id de la pagina a pinear: ";
  cin >> idPinned;

  if (option == '1') {
    pinnedPage(idPinned);
  } else {
    unpinnedPage(idPinned);
  }
}

void BufferManager::updatePage(int pageID, string& record){
  if (!bpool.isPageLoaded(pageID)) {
    cout << "La página con ID " << pageID
         << " no está cargada en el BufferPool.\n";
    return;
  }

  int frameId = bpool.getFrameId(pageID);
  Page &page = bpool.getFrame(frameId).getPage();

  cout << "(1) Añadir registro\n";
  cout << "(2) Eliminar registro\n";
  cout << "(3) Editar registro\n";
  int option;
  cin >> option;
  cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
  int index;

  switch (option) {
  case 1: // Añadir registro
    if (!page.addRecordInContent(record)) {
      cout << "No hay suficiente espacio en la página para añadir el "
              "registro.\n";
    }
    break;
  case 2: // Eliminar registro
    cout << "Ingrese el índice del registro que desea eliminar: ";
    cin >> index;
    if (!page.deleteRecordInContent(index)) {
      cout << "No existe un registro en el índice proporcionado.\n";
    }
    break;
  case 3: // Editar registro
    cout << "Ingrese el índice del registro que desea editar: ";
    cin >> index;
    cout << "Ingrese el nuevo contenido del registro: ";
    cin >> record;
    if (!page.deleteRecordInContent(index) ||
        !page.addRecordInContent(record)) {
      cout
          << "No existe un registro en el índice proporcionado o no hay "
             "suficiente espacio en la página para añadir el nuevo registro.\n";
    }
    break;
  default:
    cout << "Opción no válida.\n";
    break;
  }
}

void BufferManager::pinnedPage(int pageID) {
  bpool.getFrames()[bpool.getFrameId(pageID)].setPinned(true);
}

void BufferManager::unpinnedPage(int pageID) {
  bpool.getFrames()[bpool.getFrameId(pageID)].setPinned(false);
}

void BufferManager::showPageDetails(int pageID) {
  if (bpool.isPageLoaded(pageID)) {
    int frameId = bpool.getFrameId(pageID);
    Page page = bpool.getFrame(frameId).getPage();
    cout << "Detalles de la página:\n";
    cout << "Capacidad: " << page.getSize() << "\n";
    cout << "Contenido: " << "\n" << page.getContent() << "\n";
  } else {
    cout << "La página con ID " << pageID
         << " no está cargada en el BufferPool.\n";
  }
}

void BufferManager::addRecordInBuffer(int pageID, string record) {
  if (!bpool.isPageLoaded(pageID)) {
    cout << "La página con ID " << pageID
         << " no está cargada en el BufferPool.\n";
    return;
  }

  int frameId = bpool.getFrameId(pageID);
  Page &page = bpool.getFrame(frameId).getPage();

  if (!page.addRecordInContent(record)) {
    cout << "No hay suficiente espacio en la página para añadir el registro.\n";
  }
}

pair<bool,int> BufferManager::addRecordInBuffer1(int pageID, string record) {
  int frameId = bpool.getFrameId(pageID);
  Page &page = bpool.getFrame(frameId).getPage();
  pair<bool,int> answer = page.addRecordInContent1(record);
  return answer;
}

string BufferManager::getPathName(int pageID) {
  if (!bpool.isPageLoaded(pageID)) {
    cout << "La página con ID " << pageID
         << " no está cargada en el BufferPool.\n";
    return "";
  }

  int frameId = bpool.getFrameId(pageID);
  Page &page = bpool.getFrame(frameId).getPage();
  return page.getName();
}