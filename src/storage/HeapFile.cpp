#include "storage/HeapFile.h"

#include <regex>

HeapFile::HeapFile(string pathHF) : pathHeapFile(pathHF) {}

int extractBlockNumber(const std::string& block) {
  regex rgx(R"(block(\d+)\.txt)");
  smatch match;
  if (regex_search(block, match, rgx)) {
    return stoi(match[1].str());
  }
  return -1;
}

void HeapFile::addFreeBlock(string block) {
  //freeBlocks.push_back(block);
  int newBlockNumber = extractBlockNumber(block);
  if (newBlockNumber == -1) {
    // Handle error: Esto no es un bloque
    return;
  }

  auto it = freeBlocks.begin();
  while (it != freeBlocks.end()) {
    int currentBlockNumber = extractBlockNumber(*it);
    if (currentBlockNumber > newBlockNumber) {
      break;
    }
    ++it;
  }
  freeBlocks.insert(it, block);
}

vector<string> HeapFile::getBlocks() {
  return freeBlocks;
}

void HeapFile::saveToFileFreeBlocks() {
  ofstream file( "../../data/heapfiles/" + pathHeapFile + ".txt");
  if (!file.is_open()) {
    cerr << "HF: No se pudo abrir el archivo " << pathHeapFile << ".txt" << endl;
    return;
  }

  for (const string& block : freeBlocks) {
    file << block << endl;
  }

  file.close();
}

void HeapFile::loadFromFileFreeBlocks() {
  ifstream file( "../../data/heapfiles/" + pathHeapFile + ".txt");
  if (!file.is_open()) {
    cerr << "HF: No se pudo abrir el archivo " << pathHeapFile << ".txt" << endl;
    return;
  }

  string block;
  while (getline(file, block)) {
    freeBlocks.push_back(block);
  }

  file.close();
}

void HeapFile::addBlockToRelation(string relation, string block) {
  ofstream file( "../../data/heapfiles/" + relation + ".txt", ios::app);
  if (!file.is_open()) {
    cerr << "Add Block: No se pudo abrir el archivo " << relation << ".txt" << endl;
    return;
  }

  file << block << endl;
  file.close();
}

string HeapFile::getPathHF() const {
  return this->pathHeapFile;
}

bool HeapFile::hasFreeBlocks() const {
  return !freeBlocks.empty();
}

string HeapFile::getAndRemoveFirstBlock() {
  if (freeBlocks.empty()) {
    return "";
  }
  string firstBlock = freeBlocks.front();
  freeBlocks.erase(freeBlocks.begin());
  return firstBlock;
}
