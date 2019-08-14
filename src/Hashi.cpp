// Copyright 2018,
// Author: Luka Steinbach <luka.steinbach@gmx.de>

#include <ncurses.h>
#include <getopt.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include "./Hashi.h"

using std::vector;
using std::string;

// _____________________________________________________________________________
Hashi::Hashi() {
  _sizeX = 3;
  _sizeY = 3;
  _running = false;
  _clickCounter = 0;
  _marked1X = -1;
  _marked1Y = -1;
  _marked2X = -1;
  _marked2Y = -1;
  _steps = 0;
}

// _____________________________________________________________________________
Hashi::~Hashi() {
  for (auto& i : _currentMatchfield) {
    for (auto& j : i) {
      delete j;
    }
    i.clear();
  }
  endwin();
}

// _____________________________________________________________________________
void Hashi::initializeGame() const {
  // Init terminal
  initscr();
  cbreak();
  noecho();
  curs_set(false);
  nodelay(stdscr, true);
  keypad(stdscr, true);
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  init_pair(2, COLOR_BLACK, COLOR_GREEN);
  init_pair(3, COLOR_RED, COLOR_BLACK);
  init_pair(4, COLOR_BLACK, COLOR_RED);
  // Catch mouse events
  mousemask(ALL_MOUSE_EVENTS, NULL);
}

// _____________________________________________________________________________
void Hashi::printUsageAndExit() const {
  std::cerr << "Usage: ./HashiMain [options] <instance file>\n"
  "Available options:\n"
  "--undo-count <int> : Amount of steps you can undo.\n"
  " (default: 5)\n";
  endwin();
  exit(1);
}

// _____________________________________________________________________________
void Hashi::parseCommandLineArguments(int argc, char** argv) {
  struct option options[] = {
    {"undo-count", 1, NULL, 'u'},
    {NULL, 0, NULL, 0}
  };
  optind = 1;

  // Default values
  _undoCount = 5;
  _currentFile = "";

  while (true) {
    char c = getopt_long(argc, argv, "u:", options, NULL);
    if (c == -1) { break; }
    switch (c) {
      case 'u':
        _undoCount = atoi(optarg);
        break;
    }
  }
  // input file is required argument.
  if (optind + 1 != argc) {
    printUsageAndExit();
  }
  _currentFile = argv[optind];
}

// _____________________________________________________________________________
void Hashi::loadInstance() {
  // Load File
  string filepath = _currentFile;
  std::ifstream file(filepath.c_str());
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << filepath << std::endl;
    endwin();
    exit(1);
  }
  _currentFile = filepath;
  int fileLength = filepath.length();
  string line;
  vector<vector<string>> strMatchfield;
  int n = 0;
  while (true) {
    std::getline(file, line);
    if (file.eof()) {
      break;
    }
    n++;

    // XY
    if (filepath.substr(fileLength - 3, fileLength) == ".xy") {
      // First line
      if (line[0] == '#') {
        int state = 0;
        string s;
        for (size_t i = 2; i < line.length(); i++) {
          if (state == 0) {
            if (line[i] == ':') {
              _sizeX = stoi(s);
              s.clear();
              state++;
            } else {
              s += line[i];
            }
          } else if (state == 1) {
            if (line[i] == ' ') {
              _sizeY = stoi(s);
              break;
            } else {
              s += line[i];
            }
          }
        }
        // Fill field with bridges
        _lastMatchfields.resize(_undoCount);
        _currentMatchfield.resize(_sizeX);
        for (int i = 0; i < _sizeX; i++) {
          _currentMatchfield[i].resize(_sizeY);
        }
        for (int x = 0; x < _sizeX; x++) {
          for (int y = 0; y < _sizeY; y++) {
            _currentMatchfield[x][y] = new Bridge(x, y);
          }
        }
      // Other lines
      } else {
        int state = 0;
        string s;
        int x;
        int y;
        int count;
        for (size_t i = 0; i < line.length(); i++) {
          if (state == 0) {
            if (line[i] == ',') {
              x = stoi(s);
              s.clear();
              state++;
            } else {
              s += line[i];
            }
          } else if (state == 1) {
            if (line[i] == ',') {
              y = stoi(s);
              s.clear();
              state++;
            } else {
              s += line[i];
            }
          } else if (state == 2) {
            if (i == line.length() - 1) {
              s += line[i];
              count = stoi(s);
              s.clear();
              state++;
            } else {
              s += line[i];
            }
          }
        }
        delete _currentMatchfield[x][y];
        _currentMatchfield[x][y] = new Island(x, y, count);
      }

    // Plain
    } else if (filepath.substr(fileLength - 6, fileLength) == ".plain") {
      // First line
      if (line[0] == '#') {
        int state = 0;
        string s;
        for (size_t i = 2; i < line.length(); i++) {
          if (state == 0) {
            if (line[i] == ':') {
              _sizeX = stoi(s);
              s.clear();
              state++;
            } else {
              s += line[i];
            }
          } else if (state == 1) {
            if (line[i] == ' ') {
              _sizeY = stoi(s);
              break;
            } else {
              s += line[i];
            }
          }
        }
        // Fill field with bridges
        _lastMatchfields.resize(_undoCount);
        _currentMatchfield.resize(_sizeX);
        for (int i = 0; i < _sizeX; i++) {
          _currentMatchfield[i].resize(_sizeY);
        }
        for (int x = 0; x < _sizeX; x++) {
          for (int y = 0; y < _sizeY; y++) {
            _currentMatchfield[x][y] = new Bridge(x, y);
          }
        }
      } else {
        strMatchfield.resize(_sizeX);
        for (auto& i : strMatchfield) {
          i.resize(_sizeY);
        }
        for (size_t i = 0; i < line.length(); i++) {
          strMatchfield[i][n - 2] = line[i];
        }
      }
    } else {
      std::cerr << "Error opening file: " << filepath << std::endl;
      endwin();
      exit(1);
    }
  }
  if (filepath.substr(fileLength - 6, fileLength) == ".plain") {
    _currentMatchfield = strToGameState(strMatchfield);
  }
  // Fill _lastMatchfields with _currentMatchfield
  for (int i = 0; i < _undoCount; i++) {
    save();
  }
}

// ____________________________________________________________________________
vector<vector<string>> Hashi::gameStateToStr(vector<vector<Field*>> v) {
  vector<vector<string>> result;
  result.resize(_sizeX);
  for (auto& i : result) {
    i.resize(_sizeY);
  }

  for (int x = 0; x < _sizeX; x++) {
    for (int y = 0; y < _sizeY; y++) {
      result[x][y] = v[x][y]->getSymbol();
    }
  }
  return result;
}

// ____________________________________________________________________________
vector<vector<Field*>> Hashi::strToGameState(vector<vector<string>> v) {
  vector<vector<Field*>> result;
  result.resize(_sizeX);
  for (auto& i : result) {
    i.resize(_sizeY);
  }
  Field* current;
  for (int x = 0; x < _sizeX; x++) {
    for (int y = 0; y < _sizeY; y++) {
      if (v[x][y] == " ") {
        current = new Bridge(x, y);
      } else if (v[x][y] == "-") {
        current = new Bridge(x, y);
        current->_directionHor = true;
        current->addWeight();
      } else if (v[x][y] == "=") {
        current = new Bridge(x, y);
        current->_directionHor = true;
        current->addWeight();
        current->addWeight();
      } else if (v[x][y] == "|") {
        current = new Bridge(x, y);
        current->_directionHor = false;
        current->addWeight();
      } else if (v[x][y] == "H") {
        current = new Bridge(x, y);
        current->_directionHor = false;
        current->addWeight();
        current->addWeight();
      } else {
        current = new Island(x, y, stoi(v[x][y]));
      }
      result[x][y] = current;
    }
  }
  return result;
}

// ____________________________________________________________________________
void Hashi::loadSolution() {}

// _____________________________________________________________________________
void Hashi::play() {
  _running = true;
  while (true) {
    processUserInput(getch());
    if (_running) {
      checkIslands();
      showMatchfield();
    }
  }
}

// _____________________________________________________________________________
void Hashi::showMatchfield() const {
  attroff(A_REVERSE);
  for (int x = 0; x < _sizeX; x++) {
    for (int y = 0; y < _sizeY; y++) {
      Field* current = _currentMatchfield[x][y];
      if (current->getType() == "type_island") {
        if (current->_correctBridgeCount == 1) {
          attron(COLOR_PAIR(1));
        } else if (current->_correctBridgeCount == 2) {
          attron(COLOR_PAIR(3));
        }
      }

      if ((x == _marked1X && y == _marked1Y) ||
          (x == _marked2X && y == _marked2Y)) {
            if (current->getType() == "type_island") {
              if (current->_correctBridgeCount == 1) {
                attron(COLOR_PAIR(2));
              } else if (current->_correctBridgeCount == 2) {
                attron(COLOR_PAIR(4));
              } else {
                attron(A_REVERSE);
              }
            }
      }

      mvprintw(y, 2* x, _currentMatchfield[x][y]->getSymbol().c_str());
      attroff(COLOR_PAIR(1));
      attroff(A_REVERSE);
    }
  }
  // for(int i = 0; i < _sizeY; i++) {
  //   mvprintw(i, 60, "%s", _lastMatchfields[0][0][i].c_str());
  // }
  refresh();
}

// _____________________________________________________________________________
void Hashi::setBridge(int x1, int y1, int x2, int y2) {
  _currentClickedX = -1;
  _currentClickedY = -1;
  _lastClickedX = -1;
  _lastClickedY = -1;
  _marked1X = -1;
  _marked1Y = -1;
  _marked2X = -1;
  _marked2Y = -1;
  // Coordinates in matchfield
  if (inField(x1, y1) && inField(x2, y2) && x1 % 2 == 0 && x2 % 2 == 0) {
    x1 = x1 / 2;
    x2 = x2 / 2;
    // Coordinates in vertical row
    if (x1 == x2) {
      int x = x1;
      int minY = std::min(y1, y2);
      int maxY = std::max(y1, y2);
      if (checkRowY(minY, maxY, x)) {
        // Build bridge
        bool directionHor = false;
        save();
        for (int y = minY; y < maxY; y++) {
          _currentMatchfield[x][y]->_directionHor = directionHor;
          _currentMatchfield[x][y]->addWeight();
          _steps++;
        }
      }
    // Coordinates in horizontal row
    } else if (y1 == y2) {
      int y = y1;
      int minX = std::min(x1, x2);
      int maxX = std::max(x1, x2);
      if (checkRowX(minX, maxX, y)) {
        // Build bridge
        bool directionHor = true;
        save();
        for (int x = minX; x < maxX; x++) {
          _currentMatchfield[x][y]->_directionHor = directionHor;
          _currentMatchfield[x][y]->addWeight();
          _steps++;
        }
      }
    }
  }
}

// ____________________________________________________________________________
bool Hashi::checkRowX(int x1, int x2, int y) const {
  for (int x = x1 + 1; x < x2; x++) {
    if (_currentMatchfield[x][y]->getType() == "type_bridge" &&
        _currentMatchfield[x][y]->getCount() > 0 &&
        _currentMatchfield[x][y]->_directionHor == false) {
      return false;
    } else if (_currentMatchfield[x][y]->getType() == "type_island") {
      return false;
    }
  }
  if (_currentMatchfield[x1][y]->getType() != "type_island" ||
      _currentMatchfield[x2][y]->getType() != "type_island") {
    return false;
  }
  if (x1 == x2) {
    return false;
  }
  return true;
}

// ____________________________________________________________________________
bool Hashi::checkRowY(int y1, int y2, int x) const {
  for (int y = y1 + 1; y < y2; y++) {
    if (_currentMatchfield[x][y]->getType() == "type_bridge" &&
        _currentMatchfield[x][y]->getCount() > 0 &&
        _currentMatchfield[x][y]->_directionHor == true) {
      return false;
    } else if (_currentMatchfield[x][y]->getType() == "type_island") {
      return false;
    }
  }
  if (_currentMatchfield[x][y1]->getType() != "type_island" ||
      _currentMatchfield[x][y2]->getType() != "type_island") {
    return false;
  }
  if (y1 == y2) {
    return false;
  }
  return true;
}

// ____________________________________________________________________________
bool Hashi::inField(int x, int y) const {
  return x >= 0 && x < _sizeX * 2 && y >= 0 && y < _sizeY;
}

// _____________________________________________________________________________
void Hashi::undo() {
  if (_undoCount > 0 && _steps > 0) {
    for (auto& i : _currentMatchfield) {
      for (auto& j : i) {
        delete j;
      }
      i.clear();
    }
    _currentMatchfield.resize(_sizeX);
    for (int i = 0; i < _sizeX; i++) {
      _currentMatchfield[i].resize(_sizeY);
    }
    _currentMatchfield = strToGameState(_lastMatchfields[0]);
    checkIslands();
    for (int i = 0; i < _undoCount - 1; i++) {
      _lastMatchfields[i] = _lastMatchfields[i + 1];
    }
    _steps--;
  }
  showMatchfield();
}

void Hashi::save() {
  if (_lastMatchfields.size() > 0) {
    for (int i = _lastMatchfields.size() - 1; i > 0; i--) {
      _lastMatchfields[i] = _lastMatchfields[i - 1];
    }
    _lastMatchfields[0] = gameStateToStr(_currentMatchfield);
  }
}

// _____________________________________________________________________________
int Hashi::processUserInput(int key) {
  MEVENT event;
  if (getmouse(&event) == OK) {
    if (event.bstate & BUTTON1_CLICKED) {
      int x = event.x;  // x-coordinate of click (col index)
      int y = event.y;  // y-coordinate of click (row index)
      if (_clickCounter == 0) {
        _currentClickedX = x;
        _currentClickedY = y;
        if (x % 2 == 0 && inField(x, y)) {
          if (_currentMatchfield[x / 2][y]->getType() == "type_island") {
            _marked1X = x / 2;
            _marked1Y = y;

            _clickCounter++;
          }
        }
      } else {
        _lastClickedX = _currentClickedX;
        _lastClickedY = _currentClickedY;
        _currentClickedX = x;
        _currentClickedY = y;
        _clickCounter = 0;
        if (x % 2 == 0 && inField(x, y)) {
          if (_currentMatchfield[x / 2][y]->getType() == "type_island") {
            _marked2X = x / 2;
            _marked2Y = y;
            setBridge(_lastClickedX, _lastClickedY,
                      _currentClickedX, _currentClickedY);
          }
        } else {
          _marked1X = -1;
          _marked1Y = -1;
          _clickCounter = 0;
        }
      }
    }
  }
  switch (key) {
    case 'u':
      undo();
      break;
    case 10:
      if (_running == false) {
        endwin();
        exit(1);
      }
    case 27:
      endwin();
      exit(1);
  }
  return 0;
}

// _____________________________________________________________________________
void Hashi::checkIslands() {
  for (auto& row : _currentMatchfield) {
    for (auto& field : row) {
      if (field->getType() == "type_island") {
        int fieldCount = field->getCount();
        int count = 0;
        int fieldX = field->getX();
        int fieldY = field->getY();
        Field* current;
        for (int dx = -1; dx <= 1; dx++) {
          for (int dy = -1; dy <= 1; dy++) {
            if ((dx == 0) ^ (dy == 0)) {
              if (inField((fieldX + dx) * 2, fieldY + dy)) {
                current = _currentMatchfield[fieldX + dx][fieldY + dy];
                if (current->getType() == "type_bridge") {
                  if (dx == 0 && current->_directionHor == false) {
                    count += current->getCount();
                  } else if (dy == 0 && current->_directionHor == true) {
                    count += current->getCount();
                  }
                }
              }
            }
          }
        }
        if (count == fieldCount) {
          field->_correctBridgeCount = 1;
        } else if (count > fieldCount) {
          field->_correctBridgeCount = 2;
        } else {
          field->_correctBridgeCount = 0;
        }
      }
    }
  }

  bool allCorrect = true;
  for (auto& row : _currentMatchfield) {
    for (auto& field : row) {
      if (field->getType() == "type_island") {
        if (field->_correctBridgeCount != 1) { allCorrect = false; break;}
      }
    }
  }

  if (allCorrect == true) {
    // Game won
    mvprintw(_sizeY + 1, 0, "Congratulations! You won!");
    mvprintw(_sizeY + 2, 0, "Press ENTER to close...");
    refresh();
    _running = false;
  }
}
