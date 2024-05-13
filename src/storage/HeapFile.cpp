#include "storage/HeapFile.h"

HeapFile::HeapFile(string relation) : relation(relation) {}

void HeapFile::addBlock(string block) {
  blocks.push_back(block);
}

list<string> HeapFile::getBlocks() {
  return blocks;
}

void HeapFile::saveToFile() {
  ofstream file( "../../heapfiles/" + relation + ".txt");
  if (!file.is_open()) {
    cerr << "Error: No se pudo abrir el archivo " << relation << ".txt." << endl;
    return;
  }

  for (const string& block : blocks) {
    file << block << endl;
  }

  file.close();
}

void HeapFile::loadFromFile() {
  ifstream file(relation + ".txt");
  if (!file.is_open()) {
    cerr << "Error: No se pudo abrir el archivo " << relation << ".txt." << endl;
    return;
  }

  string block;
  while (getline(file, block)) {
    blocks.push_back(block);
  }

  file.close();
}

string HeapFile::getRelation() const {
  return this->relation;
}
