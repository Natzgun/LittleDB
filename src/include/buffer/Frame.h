#ifndef FRAME_H
#define FRAME_H

#include "Page.h"
#include <queue>

class Frame {
private:
  int frameID;
  bool dirty_flag;
  int pinCount;
  Page page;
  int refbit;
  bool pinned;
  vector<int> requerimiento;

public:
  // Constructor
  Frame(int frameID);


  // Getters
  int getframeID();

  bool isDirty();

  int getPinCount();


  Page &getPage();

  int getRefBit();

  // Setters
  void setFrameID(int id);

  void setDirtyFlag(bool dirty);

  void setPinCount(int pin);

  void setRefBit(int refBit);

  void setPage(Page pg);

  void incrementPinCount();

  void decrementPinCount();

  void setPinned(bool value);

  bool getPinned();
  void addRequest(bool req);
  
  int freeRequest();

  int posLastWrite();  

  void showVector();
};

#endif  // FRAME_H
