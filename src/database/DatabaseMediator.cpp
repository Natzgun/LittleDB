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

  string splatter = to_string(platter);
  string strack = to_string(track);
  string sblock = to_string(block);
  string blockPath = "../../data/root_directory/platter" + splatter + "/track" + strack + "/block" + sblock + ".txt";
  bfManager.loadPageFromDiskClock(blockNumber, blockPath, mode);
}

int DatabaseMediator::convertPathToPage(const string& blockPath, char mode) {
    size_t platterPos = blockPath.find("platter") + 7; 
    size_t trackPos = blockPath.find("track") + 5;     
    size_t blockPos = blockPath.find("block") + 5;     

    int platter = stoi(blockPath.substr(platterPos, blockPath.find("/", platterPos) - platterPos));
    int block = stoi(blockPath.substr(blockPos, blockPath.find(".txt", blockPos) - blockPos));
    vector<int> dataDisk1 = diskManager.getDataDisk();
    int plattersPerDisk = dataDisk1[0];
    int tracksPerPlatter = dataDisk1[1];
    int blocksPerTrack = dataDisk1[2];

    int blockNumber = (platter - 1) * tracksPerPlatter * blocksPerTrack + block;
    bfManager.loadPageFromDiskClock(blockNumber, blockPath, mode);
    return blockNumber;
}


// Erick Malcoaccha
BPlusTree & DatabaseMediator::getOrCreateBPTree(string relation) {
  auto it = bPlusTrees.find(relation);
  if (it == bPlusTrees.end()) {
    BPlusTree newTree;
    bPlusTrees[relation] = newTree;
    return bPlusTrees[relation];
  }
  return it->second;
}

// Erick Malcoaccha
string DatabaseMediator::getBlockFromBPtreeForInsert(string key, string relation, bool space) {
  BPlusTree &bptree = getOrCreateBPTree(relation);
  if((bptree.getRoot()->parent == nullptr && bptree.getRoot()->getNumKeys() == 0) || !space){ 
    diskManager.fillMapOfRelation(relation); 
    string getBlock = diskManager.getBlockToTree(relation);
    bptree.set(key, {getBlock, "0"});
    return getBlock;
  } 
  pair<string,string> getBlock = bptree.searchPolitica(bptree.getRoot(), key);
  bptree.set(key, {getBlock.first, "0"});
  return getBlock.first;
  
}

vector<string> DatabaseMediator::split(const string& str, char delimiter) {
    vector<string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }

    // Add the last token
    tokens.push_back(str.substr(start));

    // Debugging: Print tokens

    return tokens;
}

bool DatabaseMediator::findRelationInFile(string& linea,const string& relationName, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo." << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.find(relationName + "#") == 0) {
            linea = line;
            return true;
        }
    }
    return false;
}

pair<vector<string>, vector<pair<int, int>>> DatabaseMediator::parseRelation(const string& relationString) {
    // Debugging: Print the relation string

    vector<string> tokens = split(relationString, '#');

    if (tokens.size() < 4 || (tokens.size() - 1) % 3 != 0) {
        throw invalid_argument("Cadena de entrada inválida");
    }

    string relationName = tokens[0];

    vector<string> columnNames;
    vector<pair<int, int>> columnPositions;

    int currentPosition = 0;
    for (size_t i = 1; i < tokens.size(); i += 3) {
        if (i + 2 >= tokens.size()) {
            throw invalid_argument("Cadena de entrada inválida: falta información de columna");
        }
        columnNames.push_back(tokens[i]);
        int size = stoi(tokens[i + 2]);
        columnPositions.push_back(make_pair(currentPosition, currentPosition + size - 1));
        currentPosition += size;
    }

    return make_pair(columnNames, columnPositions);
}

// Erick Malcoaccha
vector<string> DatabaseMediator::getBlocksForRead(string key, string relation) {

  /*
   * Retorna la lista de bloques de una relacion en base a una condicion
   */
  return {};
}

// Erick Malcoaccha
void DatabaseMediator::fillBPtree(string relation) {
  string pathRelation = "../../data/heapfiles/" + relation + ".txt";
  ifstream file(pathRelation);

  if (!file.is_open()) {
    cout << "DBMediator: No se pudo abrir el archivo " << pathRelation << endl;
  }

}

DatabaseMediator::DatabaseMediator() : bfManager(4) {
}

// Erik Ramos Quispe
void DatabaseMediator::addRecord(string &relation, string record, bool bucle, bool end) {
  pair<bool,int> result;
  static int pageID;
  static bool first = true;
  pair<int,int> iniFin = setClave(relation);
  size_t inicio = iniFin.first;
  size_t fin = iniFin.second;
  string key = record.substr(inicio, fin - inicio + 1);
  string PathBlock = getBlockFromBPtreeForInsert(key, relation, true);
  if(end){
    first = true;
    return;
  }

  if(!bucle){
    pageID = convertPathToPage(PathBlock, 'W');
    result = bfManager.addRecordInBuffer1(pageID, record);
    if(!result.first){
      PathBlock = getBlockFromBPtreeForInsert(key, relation, false);
      pageID = convertPathToPage(PathBlock, 'W');
      bfManager.addRecordInBuffer(pageID, record);
    }
  }else{
    if(first){
      pageID = convertPathToPage(PathBlock, 'W'); 
      first = false;
    }
    result = bfManager.addRecordInBuffer1(pageID, record);
    if(!result.first){
      PathBlock = getBlockFromBPtreeForInsert(key, relation, false);
      pageID = convertPathToPage(PathBlock, 'W');
      bfManager.addRecordInBuffer(pageID, record);
    }
  }
  diskManager.updateMapOfRelationHF(relation,bfManager.getPathName(pageID), result.second);

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
  diskManager.saveMapOfRelationHF();
}

void DatabaseMediator::loadDataInFiles() {
  diskManager.loadDiskAttributesFromFile();
}

void DatabaseMediator::adminBplusTree() {
    cout << "Ingrese el nombre de la relación: " << endl;
    string nameRelation; 
    cin >> nameRelation;
    BPlusTree &tree = getOrCreateBPTree(nameRelation);

    // Exportar árbol a archivo DOT
    tree.exportToDot(nameRelation + "tree.dot");

    string searchKey;
    while (true) {
        cout << "Ingrese la clave a buscar (o 'exit' para salir): ";
        cin >> searchKey;

        if (searchKey == "exit") {
            break;
        }

        auto result = tree.search(searchKey);
        cout << "Found: " << result.first << ", " << result.second << endl;
    }

    // Imprimir el árbol por niveles
    //tree.printTreeByLevels();

    /* Código comentado para referencia y pruebas adicionales
    BPlusTree tree(3);

    // Insert example data
    tree.set("key4", {"path1", "value1"});
    tree.set("key2", {"path2", "value2"});
    tree.set("key3", {"path3", "value3"});
    tree.set("key1", {"path3", "value3"});

    tree.remove("key3");
    // Export tree to DOT file
    tree.exportToDot("tree.dot");

    //tree.remove("key2");
    // Print tree by levels
    tree.printTreeByLevels();

    // Search for a key
    auto result = tree.search("key2");
    cout << "Found: " << result.first << ", " << result.second << endl;
    */
}



pair<int,int> DatabaseMediator:: setClave(string relation){
  static map<string,pair<int,int>> longitud;
  pair<int,int> iniFin;
  bool existe = longitud.find(relation) != longitud.end();
  
  if(bPlusTrees.find(relation) == bPlusTrees.end() && !existe){
    cout << "No existe el arbol B+ para la relacion " << relation << endl;
    cout << "Elige una clave para crear el arbol B+ para la relacion " << relation << endl;
    string linea;
    if(!findRelationInFile(linea,relation)){
      cout << "No existe la relacion " << relation << endl;
      return iniFin;
    }
    pair<vector<string>, vector<pair<int,int>>> relationData = parseRelation(linea);
    for(int i = 0; i < relationData.first.size(); i++){
      cout  << i + 1 << ": " << relationData.first[i] << endl;
    }
    cout << "Elige la columa para crear el arbol B+ para la relacion " << relation << endl;
    int opcion; cin >> opcion;
    iniFin = relationData.second[opcion - 1];
    longitud[relation] = iniFin;
  }

  return longitud[relation];
}