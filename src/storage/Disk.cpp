#include "storage/Disk.h"

#include <fstream>
#include <iostream>
using namespace std;

void Disk::createFile(const string &path_file) {
  ofstream file(path_file);
  cout << "Archivo creado correctamente: " << path_file << endl;
}

void Disk::createFileBlock(const string &path_file, int numberOfBlock, int &numSector) {
  ofstream file(path_file);
  if (!file.is_open())
    return;

  file << "FREE#HEADER#Block" << numberOfBlock << endl;
  file << "sectors#";
  for (int i = 1; i <= this->sectorPerBlock; ++i, ++numSector) {
    file << "sector" << numSector;
    if (i < sectorPerBlock)
      file << "#";
  }
  numSector = numSector - this->sectorPerBlock;

  file << endl;

  file << "blockCapacity#" << this->bytesPerBlock << endl;
  for (int i = 1; i <= this->sectorPerBlock; ++i, ++numSector) {
    file << "Csector" << numSector << "#" << this->bytesPerSector << endl;
  }

  file.close();

  cout << "Bloque creado correctamente: " << path_file << endl;
}

void Disk::createDirectories(TreeNode &node, int levels, const std::vector<int> &directoriesPerLevel) {
  static int totalFiles = 0;
  static int numSector = 1;
  static int sectorNumerationForFile = 1;
  static int numBlock = 0;
  if (levels < 0)
    return;

  try {
    if (levels != 0) {
      fs::create_directory(node.directory);

      cout << "Directorio creado correctamente: " << node.directory << endl;
    } else {
      string pathSector = node.directory.string();
      size_t pos = pathSector.find_last_not_of("0123456789");
      pathSector.erase(pos + 1);
      createFile(pathSector + std::to_string(sectorNumerationForFile++) + ".txt");
      totalFiles++;

      if (totalFiles == directoriesPerLevel[0]) {
        for (int i = 0; i < this->blockPerTrack; ++i, ++numBlock) {
          //createFileBlock(node.directory.parent_path().string() + "/block" + std::to_string(numBlock + 1) + ".txt", (numBlock + 1), numSector);
          string blockPath = node.directory.parent_path().string() + "/block" + std::to_string(numBlock + 1) + ".txt";
          createFileBlock(blockPath, (numBlock + 1), numSector);

          // Crear un nuevo nodo de bloque y agregarlo al nodo actual
          TreeNode blockNode;
          blockNode.directory = blockPath;
          node.children.push_back(blockNode);
        }
        totalFiles = 0;

        // Con esto se reinicia el contador de sectores
        //numSector = 1;
      }
    }


    for (int i = 0; i < directoriesPerLevel[levels - 1]; ++i) {
      TreeNode child;
      child.directory = node.directory / (nameForFiles(levels - 1) + std::to_string(i + 1));
      node.children.push_back(child);
      createDirectories(node.children.back(), levels - 1, directoriesPerLevel);
    }
  } catch (const std::filesystem::filesystem_error &e) {
    std::cerr << "Error al crear el directorio: " << e.what() << std::endl;
  }
}

Disk::Disk() {
  // Esto es un disco de 32 Megabytes
  this->numPlatters = 2;
  this->tracksPerSurface = 64;
  this->sectorsPerTrack = 32;
  this->bytesPerSector = 4096;

  this->bytesPerBlock = 16384;
  this->sectorPerBlock = this->bytesPerBlock / this->bytesPerSector;
  this->blockPerTrack = this->sectorsPerTrack / (this->bytesPerBlock / this->bytesPerSector);
}

Disk::Disk(int plates, int tracks, int sector, int bytes, int bytesBlock) : numPlatters(plates),
                                                                            tracksPerSurface(tracks),
                                                                            sectorsPerTrack(sector),
                                                                            bytesPerSector(bytes),
                                                                            bytesPerBlock(bytesBlock) {
  this->sectorPerBlock = this->bytesPerBlock / this->bytesPerSector;
  this->blockPerTrack = this->sectorsPerTrack / (this->bytesPerBlock / this->bytesPerSector);
}

TreeNode &Disk::getRoot() {
  return this->root;
}

void Disk::capacityDisk() {
  long long cap = (this->numPlatters * 2LL) * this->tracksPerSurface * this->sectorsPerTrack * this->bytesPerSector;
  cout << "Disk capacity: " << cap << " bytes - " << cap / 1048575 << " Megabytes" << endl;
  cout << "Numero de bloques: " << blockPerTrack << endl;
  cout << "Numero de sectores por bloque: " << sectorPerBlock << endl;
}

void Disk::generateDiskStructure() {
  //TreeNode root;
  root.directory = "../../data/root_directory";

  /*
  int levels;
  cout << "Ingrese la cantidad de niveles de anidamiento: ";
  cin >> levels;
  */

  int levels = 3;
  vector<int> directoriesPerLevel(levels);
  /*
  cout << "Ingrese la cantidad de carpetas para cada nivel:" << std::endl;
  for (int i = 0; i < levels; ++i) {
    cout << "Nivel " << (i + 1) << ": ";
    cin >> directoriesPerLevel[i];
  }
  */

  directoriesPerLevel[0] = this->sectorsPerTrack;
  directoriesPerLevel[1] = this->tracksPerSurface;
  directoriesPerLevel[2] = this->numPlatters * 2;

  createDirectories(root, levels, directoriesPerLevel);

  cout << "Estructura de directorios creada exitosamente." << std::endl;
  cout << "Se ha generado el disco" << endl;
  cout << "------------------------------------------------" << endl;
}

string Disk::nameForFiles(int level) {
  if (level == 0) {
    return "sector";
  } else if (level == 1) {
    return "track";
  } else {
    return "platter";
  }
}
