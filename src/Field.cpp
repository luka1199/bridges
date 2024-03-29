// Copyright 2018,
// Author: Luka Steinbach <luka.steinbach@gmx.de>

#include "./Field.h"
#include <string>

// _____________________________________________________________________________
Field::Field(int x, int y) {
  _posX = x;
  _posY = y;
  _bordersCorrect = false;
}

// _____________________________________________________________________________
Field::~Field() {}

// _____________________________________________________________________________
int Field::getX() const {
  return _posX;
}

// _____________________________________________________________________________
int Field::getY() const {
  return _posY;
}

// ____________________________________________________________________________
int Field::getCount() const {
  return -1;
}

// _____________________________________________________________________________
std::string Field::getSymbol() const {
  return _symbol;
}

// ____________________________________________________________________________
std::string Field::getType() const {
  return "type_field";
}

// ____________________________________________________________________________
void Field::addWeight() {}
