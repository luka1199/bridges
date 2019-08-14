// Copyright 2018,
// Author: Luka Steinbach <luka.steinbach@gmx.de>

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <iostream>
#include "./Hashi.h"
#include "./Bridge.h"
#include "./Island.h"

using std::vector;
using std::string;

TEST(HashiTest, parseCommandLineArgumentsNoArguments) {
  Hashi game;
  int argc = 1;
  char* argv[1] = {
    const_cast<char*>("")
  };
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ASSERT_DEATH(game.parseCommandLineArguments(argc, argv), "Usage: .*");
}

TEST(HashiTest, parseCommandLineArguments) {
  Hashi game;

  int argc = 2;
  char* argv[2] = {
    const_cast<char*>(""),
    const_cast<char*>("InputFile")
  };
  game.parseCommandLineArguments(argc, argv);
  ASSERT_EQ(game._currentFile, "InputFile");
  ASSERT_EQ(game._undoCount, 5);
}

TEST(HashiTest, parseCommandLineArgumentsSetUndo) {
  Hashi game;

  int argc = 4;
  char* argv[4] = {
    const_cast<char*>(""),
    const_cast<char*>("--undo-count"),
    const_cast<char*>("100"),
    const_cast<char*>("InputFile")
  };
  game.parseCommandLineArguments(argc, argv);
  ASSERT_EQ(game._currentFile, "InputFile");
  ASSERT_EQ(game._undoCount, 100);
}

TEST(HashiTest, gameStateToStr) {
  Hashi game;
  Island* a = new Island(0, 0, 1);
  Island* b = new Island(0, 1, 2);
  Island* c = new Island(0, 2, 3);
  Bridge* d = new Bridge(1, 0);
  d->_directionHor = true;

  Bridge* e = new Bridge(1, 1);
  e->_directionHor = true;
  e->addWeight();

  Bridge* f = new Bridge(1, 2);
  f->_directionHor = true;
  f->addWeight();
  f->addWeight();

  Bridge* g = new Bridge(2, 0);
  g->_directionHor = false;

  Bridge* h = new Bridge(2, 1);
  h->_directionHor = false;
  h->addWeight();

  Bridge* i = new Bridge(2, 2);
  i->_directionHor = false;
  i->addWeight();
  i->addWeight();

  vector<vector<string>> str;
  vector<vector<Field*>> gs;

  game._sizeX = 0;
  game._sizeY = 0;
  ASSERT_EQ(game.gameStateToStr(gs), str);

  str = {{"1", "2", "3"}, {" ", "-", "="}, {" ", "|", "H"}};
  gs = {{a, b, c}, {d, e, f}, {g, h, i}};
  game._sizeX = 3;
  game._sizeY = 3;
  ASSERT_EQ(game.gameStateToStr(gs), str);

  for (auto& i : gs) {
    for (auto& j : i) {
      delete j;
    }
    i.clear();
  }
}

TEST(HashiTest, strToGameState) {
  Hashi game;

  Island* a = new Island(0, 0, 1);
  Island* b = new Island(0, 1, 2);
  Island* c = new Island(0, 2, 3);
  Bridge* d = new Bridge(1, 0);
  d->_directionHor = true;

  Bridge* e = new Bridge(1, 1);
  e->_directionHor = true;
  e->addWeight();

  Bridge* f = new Bridge(1, 2);
  f->_directionHor = true;
  f->addWeight();
  f->addWeight();

  Bridge* g = new Bridge(2, 0);
  g->_directionHor = false;

  Bridge* h = new Bridge(2, 1);
  h->_directionHor = false;
  h->addWeight();

  Bridge* i = new Bridge(2, 2);
  i->_directionHor = false;
  i->addWeight();
  i->addWeight();

  vector<vector<string>> str;
  vector<vector<Field*>> gs;
  vector<vector<Field*>> gs2;
  bool equalStrGs;

  game._sizeX = 0;
  game._sizeY = 0;

  equalStrGs = true;
  for (size_t x = 0; x < str.size(); x++) {
    for (size_t y = 0; y < str[x].size(); y++) {
      if (game.strToGameState(str)[x][y]->getSymbol()
          != gs[x][y]->getSymbol()) {
        equalStrGs = false;
        break;
      }
    }
  }

  ASSERT_EQ(equalStrGs, true);

  str = {{"1", "2", "3"}, {" ", "-", "="}, {" ", "|", "H"}};
  gs = {{a, b, c}, {d, e, f}, {g, h, i}};

  game._sizeX = 3;
  game._sizeY = 3;

  gs2 = game.strToGameState(str);

  equalStrGs = true;
  for (size_t x = 0; x < str.size(); x++) {
    for (size_t y = 0; y < str[x].size(); y++) {
      if (gs2[x][y]->getSymbol()
          != gs[x][y]->getSymbol()) {
        equalStrGs = false;
        break;
      }
    }
  }

  ASSERT_EQ(equalStrGs, true);
  for (auto& i : gs) {
    for (auto& j : i) {
      delete j;
    }
    i.clear();
  }

  for (auto& i : gs2) {
    for (auto& j : i) {
      delete j;
    }
    i.clear();
  }
}

TEST(HashiTest, setBridge) {
  Hashi game;

  vector<vector<string>> field;

  field = {{"2"}};
  game._sizeX = 1;
  game._sizeY = 1;
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game._currentMatchfield[0][0]->getSymbol(), "2");
  game.setBridge(0, 0, 0, 0);
  ASSERT_EQ(game._currentMatchfield[0][0]->getSymbol(), "2");

  field = {{"2"}, {" "}, {" "}};
  game._sizeX = 3;
  game._sizeY = 1;
  for (auto& i : game._currentMatchfield) { for (auto& j : i) { delete j; }}
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game._currentMatchfield[1][0]->getSymbol(), " ");
  game.setBridge(0, 0, 2 * 2, 0);
  ASSERT_EQ(game._currentMatchfield[1][0]->getSymbol(), " ");

  field = {{"2"}, {" "}, {"3"}};
  game._sizeX = 3;
  game._sizeY = 1;
  for (auto& i : game._currentMatchfield) { for (auto& j : i) { delete j; }}
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game._currentMatchfield[1][0]->getSymbol(), " ");
  game.setBridge(0, 0, 3 * 2, 0);
  ASSERT_EQ(game._currentMatchfield[1][0]->getSymbol(), " ");

  field = {{"2"}, {" "}, {"3"}};
  game._sizeX = 3;
  game._sizeY = 1;
  for (auto& i : game._currentMatchfield) { for (auto& j : i) { delete j; }}
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game._currentMatchfield[1][0]->getSymbol(), " ");
  game.setBridge(0, 0, 2 * 2, 0);
  ASSERT_EQ(game._currentMatchfield[1][0]->getSymbol(), "-");

  field = {{"2", " ", " "}};
  game._sizeX = 1;
  game._sizeY = 3;
  for (auto& i : game._currentMatchfield) { for (auto& j : i) { delete j; }}
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game._currentMatchfield[0][1]->getSymbol(), " ");
  game.setBridge(0, 0, 0, 2);
  ASSERT_EQ(game._currentMatchfield[0][1]->getSymbol(), " ");

  field = {{"2", " ", "1"}};
  game._sizeX = 1;
  game._sizeY = 3;
  for (auto& i : game._currentMatchfield) { for (auto& j : i) { delete j; }}
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game._currentMatchfield[0][1]->getSymbol(), " ");
  game.setBridge(0, 0, 0, 3);
  ASSERT_EQ(game._currentMatchfield[0][1]->getSymbol(), " ");

  field = {{"2", " ", "1"}};
  game._sizeX = 1;
  game._sizeY = 3;
  for (auto& i : game._currentMatchfield) { for (auto& j : i) { delete j; }}
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game._currentMatchfield[0][1]->getSymbol(), " ");
  game.setBridge(0, 0, 0, 2);
  ASSERT_EQ(game._currentMatchfield[0][1]->getSymbol(), "|");
}

TEST(HashiTest, checkRowX) {
  Hashi game;

  vector<vector<string>> field;

  field = {{"2"}};
  game._sizeX = 1;
  game._sizeY = 1;
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game.checkRowX(0, 0, 0), false);

  field = {{"2"}, {"-"}, {"-"}};
  game._sizeX = 3;
  game._sizeY = 1;
  for (auto& i : game._currentMatchfield) { for (auto& j : i) { delete j; }}
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game.checkRowX(0, 2, 0), false);

  field = {{"-"}, {"-"}, {"1"}};
  game._sizeX = 3;
  game._sizeY = 1;
  for (auto& i : game._currentMatchfield) { for (auto& j : i) { delete j; }}
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game.checkRowX(0, 2, 0), false);

  field = {{"1"}, {"|"}, {"1"}};
  game._sizeX = 3;
  game._sizeY = 1;
  for (auto& i : game._currentMatchfield) { for (auto& j : i) { delete j; }}
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game.checkRowX(0, 2, 0), false);

  field = {{"1"}, {"2"}, {"1"}};
  game._sizeX = 3;
  game._sizeY = 1;
  for (auto& i : game._currentMatchfield) { for (auto& j : i) { delete j; }}
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game.checkRowX(0, 2, 0), false);

  field = {{"1"}, {" "}, {"1"}};
  game._sizeX = 3;
  game._sizeY = 1;
  for (auto& i : game._currentMatchfield) { for (auto& j : i) { delete j; }}
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game.checkRowX(0, 2, 0), true);
}

TEST(HashiTest, checkRowY) {
  Hashi game;

  vector<vector<string>> field;

  field = {{"2"}};
  game._sizeX = 1;
  game._sizeY = 1;
  for (auto& i : game._currentMatchfield) { for (auto& j : i) { delete j; }}
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game.checkRowY(0, 0, 0), false);

  field = {{"2", "|", "|"}};
  game._sizeX = 1;
  game._sizeY = 3;
  for (auto& i : game._currentMatchfield) { for (auto& j : i) { delete j; }}
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game.checkRowY(0, 2, 0), false);

  field = {{"H", "H", "1"}};
  game._sizeX = 1;
  game._sizeY = 3;
  for (auto& i : game._currentMatchfield) { for (auto& j : i) { delete j; }}
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game.checkRowY(0, 2, 0), false);

  field = {{"1", "-", "1"}};
  game._sizeX = 1;
  game._sizeY = 3;
  for (auto& i : game._currentMatchfield) { for (auto& j : i) { delete j; }}
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game.checkRowY(0, 2, 0), false);

  field = {{"1", "2", "1"}};
  game._sizeX = 1;
  game._sizeY = 3;
  for (auto& i : game._currentMatchfield) { for (auto& j : i) { delete j; }}
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game.checkRowY(0, 2, 0), false);

  field = {{"1", " ", "1"}};
  game._sizeX = 1;
  game._sizeY = 3;
  for (auto& i : game._currentMatchfield) { for (auto& j : i) { delete j; }}
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game.checkRowY(0, 2, 0), true);
}

TEST(HashiTest, inField) {
  Hashi game;
  game._sizeX = 10;
  game._sizeY = 10;
  ASSERT_EQ(game.inField(0, 0), true);
  ASSERT_EQ(game.inField(5, 5), true);
  ASSERT_EQ(game.inField(19, 9), true);
  ASSERT_EQ(game.inField(19, 10), false);
  ASSERT_EQ(game.inField(-1, -1), false);
  ASSERT_EQ(game.inField(-100, 1), false);
}

TEST(HashiTest, undo) {
  Hashi game;
  Bridge* a = new Bridge(1, 1);
  Island* b = new Island(2, 1, 2);
  Island* c = new Island(4, 3, 3);

  vector< vector<Field*> > mf1 {{a}};
  vector< vector<Field*> > mf2 {{b}};
  vector< vector<Field*> > mf3 {{c}};
  vector< vector<string> > f1 {{" "}};
  vector< vector<string> > f2 {{"2"}};
  vector< vector<string> > f3 {{"3"}};

  game._undoCount = 0;
  game._steps = 2;
  game._lastMatchfields.resize(game._undoCount);
  game._sizeX = 1;
  game._sizeY = 1;
  game._currentMatchfield = mf1;
  ASSERT_EQ(game._currentMatchfield, mf1);
  game.undo();
  ASSERT_EQ(game._lastMatchfields.size(), 0);
  ASSERT_EQ(game._currentMatchfield, mf1);

  game._undoCount = 2;
  game._lastMatchfields.resize(game._undoCount);
  ASSERT_EQ(game._currentMatchfield, mf1);
  game._lastMatchfields[0] = f2;
  game._lastMatchfields[1] = f3;
  game.undo();
  ASSERT_EQ(game._lastMatchfields.size(), 2);
  ASSERT_EQ(game.gameStateToStr(game._currentMatchfield),
            game.gameStateToStr(mf2));
  ASSERT_EQ(game._lastMatchfields[0], f3);
  ASSERT_EQ(game._lastMatchfields[1], f3);

  game.undo();
  ASSERT_EQ(game.gameStateToStr(game._currentMatchfield),
            game.gameStateToStr(mf3));
  ASSERT_EQ(game._lastMatchfields[0], f3);
  ASSERT_EQ(game._lastMatchfields[1], f3);

  delete b;
  delete c;
}

TEST(HashiTest, save) {
  Hashi game;
  Bridge* a = new Bridge(1, 1);

  vector< vector<Field*> > mf {{a}};
  vector< vector<string> > f1 {{"1"}};
  vector< vector<string> > f2 {{"2"}};
  vector< vector<string> > f3 {{"3"}};

  game._currentMatchfield = mf;
  game._sizeX = 1;
  game._sizeY = 1;

  ASSERT_EQ(game._lastMatchfields.size(), 0);
  game.save();
  ASSERT_EQ(game._lastMatchfields.size(), 0);

  game._lastMatchfields.push_back(f1);
  game._lastMatchfields.push_back(f2);
  game._lastMatchfields.push_back(f3);

  ASSERT_EQ(game._lastMatchfields.size(), 3);
  ASSERT_EQ(game._lastMatchfields[0][0][0], "1");
  ASSERT_EQ(game._lastMatchfields[1][0][0], "2");
  ASSERT_EQ(game._lastMatchfields[2][0][0], "3");
  game.save();
  ASSERT_EQ(game._lastMatchfields.size(), 3);
  ASSERT_EQ(game._lastMatchfields[0][0][0], " ");
  ASSERT_EQ(game._lastMatchfields[1][0][0], "1");
  ASSERT_EQ(game._lastMatchfields[2][0][0], "2");
  game.save();
  ASSERT_EQ(game._lastMatchfields.size(), 3);
  ASSERT_EQ(game._lastMatchfields[0][0][0], " ");
  ASSERT_EQ(game._lastMatchfields[1][0][0], " ");
  ASSERT_EQ(game._lastMatchfields[2][0][0], "1");
  game.save();
  ASSERT_EQ(game._lastMatchfields.size(), 3);
  ASSERT_EQ(game._lastMatchfields[0][0][0], " ");
  ASSERT_EQ(game._lastMatchfields[1][0][0], " ");
  ASSERT_EQ(game._lastMatchfields[2][0][0], " ");
}

TEST(HashiTest, processUserInputEsc) {
  Hashi game;

  ASSERT_DEATH(game.processUserInput(27), "");
}

TEST(HashiTest, processUserInputEnter) {
    Hashi game;

    game._running = false;
    ASSERT_DEATH(game.processUserInput(10), "");
}

TEST(HashiTest, checkIslands) {
  Hashi game;
  vector<vector<string>> field;

  // _correctBridgeCount = 0; vertical
  field = {{"1", " ", "1"}};
  game._sizeX = 1;
  game._sizeY = 3;
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game._currentMatchfield[0][0]->_correctBridgeCount, 0);

  game.checkIslands();

  ASSERT_EQ(game._currentMatchfield[0][0]->_correctBridgeCount, 0);

  // _correctBridgeCount = 1; vertical
  field = {{"1", "|", "1"}};
  for (auto& i : game._currentMatchfield) { for (auto& j : i) { delete j; }}
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game._currentMatchfield[0][0]->_correctBridgeCount, 0);

  game.checkIslands();

  ASSERT_EQ(game._currentMatchfield[0][0]->_correctBridgeCount, 1);

  // _correctBridgeCount = 2; vertical
  field = {{"1", "H", "1"}};
  for (auto& i : game._currentMatchfield) { for (auto& j : i) { delete j; }}
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game._currentMatchfield[0][0]->_correctBridgeCount, 0);

  game.checkIslands();

  ASSERT_EQ(game._currentMatchfield[0][0]->_correctBridgeCount, 2);

  // _correctBridgeCount = 0; horizontal
  field = {{"1"}, {" "}, {"1"}};
  game._sizeX = 3;
  game._sizeY = 1;
  for (auto& i : game._currentMatchfield) { for (auto& j : i) { delete j; }}
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game._currentMatchfield[0][0]->_correctBridgeCount, 0);

  game.checkIslands();

  ASSERT_EQ(game._currentMatchfield[0][0]->_correctBridgeCount, 0);

  // _correctBridgeCount = 1; horizontal
  field = {{"1"}, {"-"}, {"1"}};
  for (auto& i : game._currentMatchfield) { for (auto& j : i) { delete j; }}
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game._currentMatchfield[0][0]->_correctBridgeCount, 0);

  game.checkIslands();

  ASSERT_EQ(game._currentMatchfield[0][0]->_correctBridgeCount, 1);

  // _correctBridgeCount = 2; horizontal
  field = {{"1"}, {"="}, {"1"}};
  for (auto& i : game._currentMatchfield) { for (auto& j : i) { delete j; }}
  game._currentMatchfield = game.strToGameState(field);
  ASSERT_EQ(game._currentMatchfield[0][0]->_correctBridgeCount, 0);

  game.checkIslands();

  ASSERT_EQ(game._currentMatchfield[0][0]->_correctBridgeCount, 2);
}
