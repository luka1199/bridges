// Copyright 2018,
// Author: Luka Steinbach <luka.steinbach@gmx.de>

#include <gtest/gtest.h>
#include "./Hashi.h"

TEST(BridgeTest, addWeight) {
  Bridge bridge(0, 0);
  ASSERT_EQ(bridge.getCount(), 0);
  ASSERT_EQ(bridge.getSymbol(), " ");
  bridge.addWeight();
  ASSERT_EQ(bridge.getCount(), 1);
  ASSERT_EQ(bridge.getSymbol(), "|");
  bridge.addWeight();
  ASSERT_EQ(bridge.getCount(), 2);
  ASSERT_EQ(bridge.getSymbol(), "H");
  bridge._directionHor = true;
  bridge.addWeight();
  ASSERT_EQ(bridge.getCount(), 0);
  ASSERT_EQ(bridge.getSymbol(), " ");
  bridge.addWeight();
  ASSERT_EQ(bridge.getCount(), 1);
  ASSERT_EQ(bridge.getSymbol(), "-");
  bridge.addWeight();
  ASSERT_EQ(bridge.getCount(), 2);
  ASSERT_EQ(bridge.getSymbol(), "=");
}
