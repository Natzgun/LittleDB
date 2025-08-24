#include "storage/Disk_manager.h"

#include <algorithm>
#include <iostream>
#include <filesystem>
#include <regex>

namespace fs = std::filesystem;
using namespace std;

Disk_manager::Disk_manager() : heapFile("FreeBlocksHF"), rootNode(disk.getRoot()) {
}

void Disk_manager::selectDiskStructure(bool defaultDisk) {
  if (defaultDisk) {
    dataDisk.push_back(4);
    dataDisk.push_back(8);
    dataDisk.push_back(2);
    disk.generateDiskStructure();
    disk.capacityDisk();
    /* Despues de generar la estructura del disco custom carga
     * el heapFile con los bloques libres
     */
    findFreeBlock();
    heapFile.saveToFileFreeBlocks();
  } else {
    int plates, tracks, sector, bytes, bytesPerBlock;
    cout << "Ingrese el número de platos: ";
    cin >> plates;
    cout << "Ingrese el número de pistas por superficie: ";
    cin >> tracks;
    cout << "Ingrese el número de sectores por pista: ";
    cin >> sector;
    cout << "Ingrese el número de bytes por sector: ";
    cin >> bytes;
    cout << "Ingrese el numero de bytes por bloque: ";
    cin >> bytesPerBlock;

    int sectorPerBlock = bytesPerBlock/bytes;
    int blockPerTrack = sector/sectorPerBlock;
    dataDisk.push_back(plates*2);
    dataDisk.push_back(tracks);
    dataDisk.push_back(blockPerTrack);

    disk = Disk(plates, tracks, sector, bytes, bytesPerBlock);
    disk.generateDiskStructure();
    disk.capacityDisk();

    /* Despues de generar la estructura del disco custom carga
     * el heapFile con los bloques libres
     */
    findFreeBlock();
    heapFile.saveToFileFreeBlocks();
  }
}

void Disk_manager::getCapacityDisk() {
  disk.capacityDisk();
}

string eliminarSubstring(string& str, const string& substr) {
  regex pattern(substr + "\\d*\\.txt");
  str = regex_replace(str, pattern, "");
  return str;
}

void Disk_manager::insertRecord(string &relation, string &record, int recordSize) {
  string blocksUsedPath = "../../data/heapfiles/" + relation + ".txt";
  ifstream bUsed(blocksUsedPath);

  string blockPath;
  if (bUsed.is_open()){
    getline(bUsed, blockPath);
    bUsed.close();
  } else {
    blockPath = heapFile.getAndRemoveFirstBlock();
    HeapFile::addBlockToRelation(relation,blockPath);
    heapFile.saveToFileFreeBlocks();
  }

  if (blockPath.empty()) {
    cerr << "Error: No se encontró un bloque libre." << endl;
    return;
  }

  ofstream file(blockPath, std::ios::app);

  file << record << endl;

  file.close();

  cout << "Datos insertados en " + blockPath + " exitosamente Bv." << endl;

}

void Disk_manager::findFreeBlock() {
  searchFreeBlockInTree(rootNode);
}

void Disk_manager::searchFreeBlockInTree(const TreeNode &node) {
  if (isBlockFree(node.directory)) {
    string pathWithCapacity = to_string(disk.getBytesPerBlock()) + "#" + node.directory.string();
    heapFile.addFreeBlock(pathWithCapacity);
  }

  for (const auto &child : node.children) {
    searchFreeBlockInTree(child);
  }
}

bool Disk_manager::isBlockFree(const fs::path &blockPath) {
  ifstream blockFile(blockPath);

  if (blockFile.is_open()) {
    string firstLine;
    getline(blockFile, firstLine);

    blockFile.close();

    return (firstLine.find("FREE") != string::npos);
  }

  return false;
}

void Disk_manager::writeRecordInSectors(string &relation) {
  /* Aqui abro la relacion para saber de que bloque quiero escribir en el Disco (sectores)*/
  string blocksUsedPath = "../../data/heapfiles/" + relation + ".txt";
  ifstream bUsed(blocksUsedPath);

  string blockPath;
  if (bUsed.is_open()) {
    getline(bUsed, blockPath);
    bUsed.close();
  }

  /* Se optiene el nombre de la ruta para poder abrir los sectores correspondientes
   * y escribir en ellos
   */
  ifstream blockFile(blockPath);
  blockPath = eliminarSubstring(blockPath, "/block");

}


string Disk_manager::openBlock(const string &blockPath, const string &relationName) {
  fstream blockFile(blockPath, ios::in | ios::out);

  if (!blockFile.is_open()) {
    cerr << "Error: No se pudo abrir el archivo " << blockPath << "." << endl;
    return "";
  }

  string firstLine;
  getline(blockFile, firstLine);

  if (firstLine == "FREE") {
    blockFile.seekp(0);
    blockFile << relationName;
  }

  blockFile.close();

  return blockPath;
}

bool Disk_manager::checkSpaceInBlock(const string &blockPath, int recordSize) {
  ifstream blockFile(blockPath);

  if (!blockFile.is_open()) {
    cerr << "DM: No se pudo abrir el archivo " << blockPath << "." << endl;
    return false;
  }

  string line;
  while (getline(blockFile, line)) {
    if (line.substr(0, 13) == "blockCapacity") {
      int blockCapacity = stoi(line.substr(14));
      blockFile.close();
      return blockCapacity > recordSize;
    }
  }

  blockFile.close();
  return false;
}

string Disk_manager::redirectSectorWithSpace(const string &blockPath, int recordSize) {
  ifstream blockFile(blockPath);

  if (!blockFile.is_open()) {
    cerr << "Error: No se pudo abrir el archivo " << blockPath << "." << endl;
    return "";
  }

  string line;
  bool encontrado = false;

  while (getline(blockFile, line) && !encontrado) {
    if (line.substr(0, 1) == "C") {
      line = line.substr(1);
      int sectorCapacity = stoi(line.substr(line.find("#") + 1));
      if (sectorCapacity > recordSize) {
        blockFile.close();
        encontrado = true;
        return blockPath + "/" + line.substr(0, line.find("#"));
      }
    }
  }

  blockFile.close();
  return "";
}

vector<int> Disk_manager::getDataDisk() {
  return dataDisk;
}

void Disk_manager::setBlockToRelation(const string &relation) {
  int it = disk.getNumOfPlatters();
  string blockPath;
  for(int i = 0; i < it; i++) {
    blockPath = heapFile.getAndRemoveFirstBlock();
    heapFile.addBlockToRelation(relation, blockPath);
  }
}

void Disk_manager::saveFreeBlocks() {
  heapFile.saveToFileFreeBlocks();
}

void Disk_manager::saveDiskAttributesToFile(string filename) {
  string ruta = "../../data/metadataDisk/" + filename;
  vector<int> datos = disk.getDatosDisk();
  std::ofstream file(ruta, std::ios::out | std::ios::trunc);
  if (file.is_open()) {
    for(int i = 0; i < datos.size(); i++) {
      file << datos[i] << '\n';
    }
    file.close();
  } 
  else {
    std::cerr << "No se pudo abrir el archivo para escribir.\n";
  }
}

void Disk_manager::loadDiskAttributesFromFile(string filename) {
    heapFile.loadFromFileFreeBlocks();
    string ruta = "../../data/metadataDisk/" + filename;
    std::ifstream file(ruta);
    if (file.is_open()) {
        int plates, tracks, sectors, bytes, bytesPerBlock;
        file >> plates;
        file >> tracks;
        file >> sectors;
        file >> bytes;
        file >> bytesPerBlock;

        int sectorPerBlock = bytesPerBlock / bytes;
        int blockPerTrack = sectors / sectorPerBlock;
        dataDisk.clear();
        dataDisk.push_back(plates * 2);
        dataDisk.push_back(tracks);
        dataDisk.push_back(blockPerTrack);

        disk = Disk(plates, tracks, sectors, bytes, bytesPerBlock);
        disk.capacityDisk();
        findFreeBlock();
        file.close();
    } else {
        std::cerr << "No se pudo abrir el archivo para leer. Se iniciará con los valores predeterminados.\n";
    }
}

vector<string> Disk_manager::splitCapacityAndPath(string blockPath) {
  vector<string> result;
  size_t pos = blockPath.find("#");
  if (pos != string::npos) {
    result.push_back(blockPath.substr(0, pos));
    result.push_back(blockPath.substr(pos + 1));
  }
  return result;
}

void Disk_manager::fillMapOfRelation(const std::string &relation) {
    std::string blocksUsedPath = "../../data/heapfiles/" + relation + ".txt";
    std::ifstream bUsed(blocksUsedPath);
     
    if (!bUsed.is_open()) {
        setBlockToRelation(relation); 
        bUsed.open(blocksUsedPath);
    }
     
    string line;
    bool relationExists = (mapOfRelationHF.find(relation) != mapOfRelationHF.end());

    while (std::getline(bUsed, line)) {
        vector<string> dataOfBlockHF = splitCapacityAndPath(line);
        string key = dataOfBlockHF[1];
        int slot = std::stoi(dataOfBlockHF[0]);
        if (!relationExists || 
        mapOfRelationHF[relation].find(key) == mapOfRelationHF[relation].end()) {

          mapOfRelationHF[relation][key] = slot;

        }
    }

    bUsed.close();
}


string Disk_manager::getBlockToTree(const string relation) {
    auto it = mapOfRelationHF.find(relation);
    if (it == mapOfRelationHF.end()) {
      setBlockToRelation(relation);
    }
    int maxCapacity; 
    {
      vector<int> dataDiskBit = disk.getDatosDisk();
      int bitPerBlock = dataDiskBit[4];
      maxCapacity = bitPerBlock;
    }
    const auto& innerMap = it->second;
    for (const auto& innerPair : innerMap) {
        const string path = innerPair.first;
        int currentSlot = innerPair.second;
        if (currentSlot == maxCapacity) {
          return path;
        }
    }
    setBlockToRelation(relation);
    return getBlockToTree(relation); 
}

void Disk_manager::updateMapOfRelationHF(const string &relation, const string &blockPath, int updateSlot) {
    mapOfRelationHF[relation][blockPath] = updateSlot;
}

void Disk_manager::saveMapOfRelationHF() {
    for (const auto& relation : mapOfRelationHF) {
        const string& relationName = relation.first;
        const auto& innerMap = relation.second;
        string blocksUsedPath = "../../data/heapfiles/" + relationName + ".txt";
        std::ofstream bUsed(blocksUsedPath, std::ios::out | std::ios::trunc);
        for (const auto& innerPair : innerMap) {
            const string& path = innerPair.first;
            int slot = innerPair.second;
            bUsed << slot << "#" << path << '\n';
        }
        bUsed.close();
    } 
}


vector<string> Disk_manager::getBlockPaths(string filePath) {
  ifstream file(filePath);
  vector<string> blockPaths;
  string line;

  while (getline(file, line)) {
    size_t pos = line.find('#');
    if (pos != string::npos) {
      blockPaths.push_back(line.substr(pos + 1));
    }
  }

  return blockPaths;
}

vector<string> Disk_manager::identifySectorsForBlock(string blockPath) {
  ifstream file(blockPath);
  vector<string> sectors;
  string line;
  size_t lastSlash = blockPath.find_last_of('/');
  string basePath = blockPath.substr(0, lastSlash + 1);

  while (getline(file, line)) {
    if (line.find("sectors") != string::npos) {
      istringstream iss(line);
      string token;
      getline(iss, token, '#'); // Saltar la palabra "sectors"
      while (getline(iss, token, '#')) {
        if (token[0] == 'C') {
          token = token.substr(1); // Eliminar la "C" del nombre del sector
        }
        sectors.push_back(basePath + token);
      }
    }
  }

  return sectors;
}

void Disk_manager::distributeBlocksInSectors(string relation) {
  string relationFilePath = "../../data/heapfiles/" + relation + ".txt";
  vector<string> blockPaths = getBlockPaths(relationFilePath);

  for (const string& blockPath : blockPaths) {
    vector<string> sectors = identifySectorsForBlock(blockPath);

    // Leer el contenido del bloque
    ifstream blockFile(blockPath);
    string line;
    bool isDataSection = false;
    vector<string> records;

    while (getline(blockFile, line)) {
      if (line.empty()) continue;
      if (line.find("sectors") != string::npos || line.find("blockCapacity") != string::npos || line.find("FREE#HEADER") != string::npos) {
        isDataSection = false;
      } else if (isdigit(line[0])) {
        isDataSection = true;
      }

      if (isDataSection) {
        records.push_back(line);
      }
    }

    // Distribuir los registros en los sectores
    size_t sectorIndex = 0;
    for (const string& record : records) {
      string sectorPath = sectors[sectorIndex];
      ofstream sectorFile(sectorPath, ios_base::app); // Append mode
      sectorFile << record << endl;
      sectorIndex = (sectorIndex + 1) % sectors.size();
    }
  }
}
