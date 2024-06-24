#include "database/DatabaseMediator.h"
#include "buffer/BufferManager.h"

DatabaseMediator::DatabaseMediator() : bfManager(4) {
}

void DatabaseMediator::addRecord(string &relation, string record) {
  int pageID;
  cout << "ID de la pagina: ";
  cin >> pageID;
  bfManager.addRecordInBuffer(pageID, record);
}

void DatabaseMediator::adminRam() {
  while (true) {
    int instruction;
    cout << "================================================================"
        << endl;
    cout << "1. Load page " << endl;
    cout << "2. UpdatePage " << endl;
    cout << "3. Kill Process " << endl;
    cout << "4. Save page " << endl;
    cout << "5. Print Table Frame" << endl;
    cout << "6. Pin" << endl;
    cout << "7. Exit" << endl;
    cout << "8. Show Page Details" << endl;
    cout << "================================================================"
        << endl;
    cout << "Option: ";
    cin >> instruction;
    switch (instruction) {
      case 1: {
        cout << "En que modo desea cargar" << endl;
        cout << "(L) Lectura" << endl;
        cout << "(W) Escritura" << endl;
        char mode;
        cout << "Mode: ";
        cin >> mode;
        int pageid;
        cout << "Page ID: ";
        cin >> pageid;
        string blockPath;
        cout << "Block Path: ";
        cin >> blockPath;
        bfManager.loadPageFromDiskClock(pageid, blockPath, mode);
        break;
      }
      case 2: {
        int pageID;
        // cout << "Page ID: ";
        // cin >> pageID;
        // bfManger.updatePage(pageID);
        break;
      }
      case 3: {
        cout << "Indica el proceso que deseas terminar" << endl;
        int pageid;
        cout << "Page ID: ";
        cin >> pageid;
        bfManager.killProcess(pageid);
        break;
      }
      case 4: {
        cout << "Indica la pagina que deseas guardar" << endl;
        int pageid;
        cout << "Page ID: ";
        cin >> pageid;
        // Aquí deberías añadir la implementación de guardar la página
        break;
      }
      case 5:
        bfManager.printTableFrame();
        break;
      case 6:
        bfManager.updatePinnedPage();
        break;
      case 7:
        return;
      case 8: {
        cout << "Indica la pagina que deseas ver" << endl;
        int pageid;
        cout << "Page ID: ";
        cin >> pageid;
        bfManager.showPageDetails(pageid);
        break;
      }
      default:
        cout << "Opción no válida, intente de nuevo." << endl;
        break;
    }
  }
}

void DatabaseMediator::selectDiskStructureMediator(bool defaultDisk) {
  diskManager.selectDiskStructure(defaultDisk);
}
