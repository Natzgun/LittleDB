#ifndef CLOCKP_H
#define CLOCKP_H
//ERICK MALCOACCHA Y SEBASTIAN MENDOZA
#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

#define FOR_LOOP(val, i, len) for (int i = val; i < len; ++i)
#define REVERSE_FOR_LOPP(i, start, end) for (int i = (end)-1; i >= start; i--)
using namespace std;

class clockP {
  int handClock;
  int size;

public:
  clockP(int size);
  clockP();
  int getSize();
  int getHandClock();
  void setSize(int size);
  void setHandClock(int hc);
  void incrementHC();
  void decrementHC();
};
#endif