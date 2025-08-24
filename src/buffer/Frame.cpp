#include "buffer/Frame.h"
#include <iostream>
/* Jose Paredes */
// Constructor
Frame::Frame(int frameID)
    : frameID(frameID), dirty_flag(false), pinCount(0), refbit(-1),
      pinned(false) {}

// Getter for frameID
int Frame::getframeID() { return frameID; }

// Getter for page
Page &Frame::getPage() { return page; }

int Frame ::getRefBit() { return refbit; }

// Setter for page
void Frame::setPage(Page page) { this->page = page; }

// Getter for dirty flag
bool Frame::isDirty() { return dirty_flag; }

// Setter for dirty flag
void Frame::setDirtyFlag(bool dirty_flag) { this->dirty_flag = dirty_flag; }

// Getter for pinCount
int Frame::getPinCount() { return pinCount; }

void Frame ::setPinCount(int value) { pinCount = value; }
void Frame ::setRefBit(int refBit) { this->refbit = refBit; }

// Increment pinCount
void Frame::incrementPinCount() { pinCount++; }

// Decrement pinCount
void Frame::decrementPinCount() { pinCount--; }

void Frame ::addRequest(bool req) { requerimiento.push_back(req); }

int Frame ::freeRequest() {
  if (!requerimiento.empty()) {
    int dirty = requerimiento.front();
    requerimiento.erase(requerimiento.begin());
    return dirty;
  }
  return -1;
}

// Set Pinned
void Frame::setPinned(bool value) { pinned = value; }

// Get Pinned
bool Frame::getPinned() { return pinned; }

int Frame ::posLastWrite() {
  int pos = -1;
  for (int i = 0; i < requerimiento.size(); i++) {
    if (requerimiento[i] == 1) {
      pos = i;
    }
  }
  return pos;
}

void Frame ::showVector() {
  for (int i = 0; i < requerimiento.size(); i++) {
    cout << requerimiento[i] << " ";
  }
  cout << "\n";
}

string Frame :: getPathName() { return page.getName(); }