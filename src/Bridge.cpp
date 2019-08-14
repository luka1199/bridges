// Copyright 2018,
// Author: Luka Steinbach <luka.steinbach@gmx.de>

#include "./Bridge.h"
#include <string>
#include <vector>

// _____________________________________________________________________________
Bridge::Bridge(int x, int y) : Field(x, y) {
  _weight = 0;
  _directionHor = false;
  _symbol = _symbolsHor[0];
}

// _____________________________________________________________________________
Bridge::~Bridge() {}

// _____________________________________________________________________________
void Bridge::addWeight() {
  _weight = (_weight + 1) % 3;
  if (_directionHor) {
    _symbol = _symbolsHor[_weight];
  } else {
    _symbol = _symbolsVer[_weight];
  }
}

// _____________________________________________________________________________
int Bridge::getCount() const {
  return _weight;
}

// _____________________________________________________________________________
std::string Bridge::getType() const {
  return "type_bridge";
}
