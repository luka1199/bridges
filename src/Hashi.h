// Copyright 2018,
// Author: Luka Steinbach <luka.steinbach@gmx.de>

#ifndef HASHI_H_
#define HASHI_H_

#include <gtest/gtest.h>
#include <vector>
#include <string>

#include "./Bridge.h"
#include "./Island.h"

using std::vector;
using std::string;

class Hashi {
 public:
  // Constructor
  Hashi();

  // Destructor
  ~Hashi();

  // Initializes ncurses
  void initializeGame() const;

  // Parse command line.
  void parseCommandLineArguments(int argc, char** argv);
  FRIEND_TEST(HashiTest, parseCommandLineArguments);
  FRIEND_TEST(HashiTest, parseCommandLineArgumentsSetUndo);

  // Loads instance
  void loadInstance();

  // Converts given vector of pointers to vector of strings representing
  // a game state
  vector<vector<string>> gameStateToStr(vector<vector<Field*>> v);
  FRIEND_TEST(HashiTest, gameStateToStr);

  // Converts given vector of strings representing a game state to a vector
  // of actual pointers
  vector<vector<Field*>> strToGameState(vector<vector<string>> v);
  FRIEND_TEST(HashiTest, strToGameState);


  // Loads solution for current instance from folder "instances"
  void loadSolution();

  // Starts the game
  void play();

 private:
  // Shows current matchfield on screen
  void showMatchfield() const;

  // Sets a bridge between two positions (checks if bridge between the two
  // positions is valid)
  void setBridge(int x1, int y1, int x2, int y2);
  FRIEND_TEST(HashiTest, setBridge);

  // Help functions for setBridge():
  bool checkRowX(int x1, int x2, int y) const;
  FRIEND_TEST(HashiTest, checkRowX);
  bool checkRowY(int y1, int y2, int x) const;
  FRIEND_TEST(HashiTest, checkRowY);
  bool inField(int x, int y) const;
  FRIEND_TEST(HashiTest, inField);

  // Undos the last step
  void undo();
  FRIEND_TEST(HashiTest, undo);

  // Saves currentMatchfield in field history
  void save();
  FRIEND_TEST(HashiTest, save);

  // Manages key and mouse events
  int processUserInput(int key);
  FRIEND_TEST(HashiTest, processUserInputEsc);
  FRIEND_TEST(HashiTest, processUserInputEnter);

  void printUsageAndExit() const;

  // Checks every island for correct bridge count on borders
  void checkIslands();
  FRIEND_TEST(HashiTest, checkIslands);

  vector< vector<Field*> > _currentMatchfield;
  vector< vector< vector<string> > > _lastMatchfields;
  int _sizeX;
  int _sizeY;
  int _clickCounter;
  int _currentClickedX;
  int _currentClickedY;
  int _lastClickedX;
  int _lastClickedY;
  int _marked1X;
  int _marked1Y;
  int _marked2X;
  int _marked2Y;
  bool _running;
  int _undoCount;
  size_t _steps;
  string _currentFile;
};

#endif  // HASHI_H_
