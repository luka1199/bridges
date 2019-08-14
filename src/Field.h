// Copyright 2018,
// Author: Luka Steinbach <luka.steinbach@gmx.de>

#ifndef FIELD_H_
#define FIELD_H_

#include <string>
#include <vector>

class Field {
 public:
  // Constructor
  Field(int x, int y);

  // Destructor
  virtual ~Field();

  // Returns x-position
  int getX() const;

  // Returns y-position
  int getY() const;

  // Returns field type of object
  virtual std::string getType() const;

  // Returns Symbol of Bridge
  virtual std::string getSymbol() const;

  // Returns count of field
  virtual int getCount() const;

  // Adds weight (1) to the bridge
  virtual void addWeight();

  int _posX;
  int _posY;

  bool _bordersCorrect;

  // True if direction of Bridge is horizontal
  bool _directionHor;

  // 1 if amount of bridges on borders = _islandCount, 2 if to many bridges
  // on borders
  int _correctBridgeCount;

  std::string _symbol;
};

#endif  // FIELD_H_
