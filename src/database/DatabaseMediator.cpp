#include "database/DatabaseMediator.h"
#include "buffer/BufferManager.h"

// Erick Malcoaccha
void DatabaseMediator::loadBlockMediator(int blockNumber, char mode) {
  vector<int> dataDisk = diskManager.getDataDisk();
  int plattersPerDisk = dataDisk[0];
  int tracksPerPlatter = dataDisk[1];
  int blocksPerTrack = dataDisk[2];

  int totalBlocksPerDisk = plattersPerDisk * tracksPerPlatter * blocksPerTrack;

  // Calcular el disco
  int disk = (blockNumber - 1) / totalBlocksPerDisk + 1;
  int blockWithinDisk = (blockNumber - 1) % totalBlocksPerDisk;

  // Calcular el plato
  int plattersPerDiskOffset = blockWithinDisk / (tracksPerPlatter * blocksPerTrack);
  int platter = plattersPerDiskOffset + 1;

  // Calcular la pista
  int trackOffset = blockWithinDisk % (tracksPerPlatter * blocksPerTrack);
  int track = trackOffset / blocksPerTrack + 1;

  // Calcular el bloque
  int block = blockNumber%(tracksPerPlatter * blocksPerTrack);
  if(block == 0){
    block = tracksPerPlatter * blocksPerTrack;
  }

  // string blockPath =  "../../data/root_directory/platter" + to_string(platter) + "/track" + to_string(track) + "/block" + to_string(block) + ".txt";
  string splatter = to_string(platter);
  string strack = to_string(track);
  string sblock = to_string(block);
  string blockPath = "../../data/root_directory/platter" + splatter + "/track" + strack + "/block" + sblock + ".txt";
  bfManager.loadPageFromDiskClock(blockNumber, blockPath, mode);
}

void DatabaseMediator::convertPathToPage(const string& blockPath, char mode) {
    size_t platterPos = blockPath.find("platter") + 7; 
    size_t trackPos = blockPath.find("track") + 5;     
    size_t blockPos = blockPath.find("block") + 5;     

    int platter = stoi(blockPath.substr(platterPos, blockPath.find("/", platterPos) - platterPos));
    int block = stoi(blockPath.substr(blockPos, blockPath.find(".txt", blockPos) - blockPos));
    vector<int> dataDisk = diskManager.getDataDisk();
    int plattersPerDisk = dataDisk[0];
    int tracksPerPlatter = dataDisk[1];
    int blocksPerTrack = dataDisk[2];

    int blockNumber = (platter - 1) * tracksPerPlatter * blocksPerTrack + block;
    bfManager.loadPageFromDiskClock(blockNumber, blockPath, mode);
}


DatabaseMediator::DatabaseMediator() : bfManager(4) {
}

// Erik Ramos Quispe
void DatabaseMediator::addRecord(string &relation, string record, bool bucle, bool end) {
  static map<string, bool> relationMap;
  auto it = relationMap.find(relation);
  if (it == relationMap.end()) {
    relationMap[relation] = true; 
    diskManager.setBlockToRelation(relation);
  }
  static int pageID = -1;
  static bool first = true;

  if(end){
    first = true;
    pageID = -1;
    return;
  }

  if(!bucle){
    cout << "ID de la pagina: ";
    cin >> pageID;
    bfManager.addRecordInBuffer(pageID, record);
  }else{
    if(first){
      cout << "ID de la pagina: ";
      cin >> pageID;
      first = false;
    }
    bfManager.addRecordInBuffer(pageID, record);
  }
}

// Sebastian Mendoza y Sergio Castillo
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
        loadBlockMediator(pageid, mode);
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

void DatabaseMediator::saveDataInRAM() {
  diskManager.saveFreeBlocks();
  diskManager.saveDiskAttributesToFile();
}

void DatabaseMediator::loadDataInFiles() {
  diskManager.loadDiskAttributesFromFile();
}
