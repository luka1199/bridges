// Copyright 2018,
// Author: Luka Steinbach <luka.steinbach@gmx.de>

#include "./Hashi.h"

int main(int argc, char** argv) {
  Hashi game;
  game.parseCommandLineArguments(argc, argv);
  game.initializeGame();
  game.loadInstance();
  game.play();
  return 0;
}
