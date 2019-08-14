// Copyright 2018,
// Author: Luka Steinbach <luka.steinbach@gmx.de>

#ifndef ISLAND_H_
#define ISLAND_H_

#include <string>
#include <vector>
#include "./Field.h"

class Island : public Field {
 public:
  // Constructor
  Island(int x, int y, int count);

  // Destructor
  ~Island();

  // Returns count of island (1 - 8)
  int getCount() const;

  // Returns field type of object
  std::string getType() const;

 private:
  int _islandCount;
};

#endif  // ISLAND_H_
