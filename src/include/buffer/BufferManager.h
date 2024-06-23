#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H

/*Erick Malcoaccha | Jose Paredes*/

#include <iostream>

#include "BufferPool.h"

class BufferManager {
private:
  int numFrames;
  BufferPool bpool;

public:
  // Constructor
  BufferManager(int numFrames);

  // Métodos
  void loadPageFromDisk(string blockPath, int pageID, char _mode);

  void killProcess(int pageID);

  void savePageToDisk(int pageID);

  void updatePinnedPage();

  void updatePage(int pageID);

  void printTableFrame();

  void loadPageFromDiskClock(int, string, char);

  void pinnedPage(int pageID);

  void unpinnedPage(int pageID);

  void showPageDetails(int pageID);
};

#endif  // BUFFERMANAGER_H
