#include "buffer/Clock.h"
clockP ::clockP(int size) {
  this->size = size;
  this->handClock = 0;
}

clockP ::clockP() {
  this->size = 0;
  this->handClock = 0;
}

int clockP ::getSize() { return size; }

int clockP ::getHandClock() { return handClock; }

void clockP ::setSize(int size) { this->size = size; }

void clockP ::setHandClock(int hc) { this->handClock = hc; }

void clockP ::incrementHC() {
  handClock++;
  if (handClock >= size) {
    handClock = 0;
  }
}

void clockP ::decrementHC() {
  handClock--;
  if (handClock < 0) {
    handClock = size - 1;
  }
}
