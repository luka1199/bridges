// Copyright 2018,
// Author: Luka Steinbach <luka.steinbach@gmx.de>

#include "./Island.h"
#include <string>
#include <vector>

// _____________________________________________________________________________
Island::Island(int x, int y, int count) : Field(x, y) {
  _islandCount = count;
  _correctBridgeCount = 0;
  _symbol = std::to_string(_islandCount);
}

// _____________________________________________________________________________
Island::~Island() {}

// _____________________________________________________________________________
int Island::getCount() const {
  return _islandCount;
}

// _____________________________________________________________________________
std::string Island::getType() const {
  return "type_island";
}
