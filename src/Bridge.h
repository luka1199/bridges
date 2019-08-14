// Copyright 2018,
// Author: Luka Steinbach <luka.steinbach@gmx.de>

#ifndef BRIDGE_H_
#define BRIDGE_H_

#include <string>
#include <vector>
#include "./Field.h"

class Bridge : public Field {
 public:
  // Constructor
  Bridge(int x, int y);

  // Destructor
  ~Bridge();

  // Adds weight (1) to the bridge
  void addWeight();

  // Returns current weight of bridge (0, 1 or 2)
  int getCount() const;

  // Returns field type of object
  std::string getType() const;

 private:
  int _weight;

  std::vector<std::string> _symbolsHor {" ", "-", "="};
  std::vector<std::string> _symbolsVer {" ", "|", "H"};
};

#endif  // BRIDGE_H_
