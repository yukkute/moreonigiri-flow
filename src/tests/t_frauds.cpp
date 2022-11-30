/**
  @file t_frauds.cpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "game.hpp"

#include <gtest/gtest.h>
#include <vector>

TEST(Frauds, FraudEarnCheated) {
  Game::createNewInstance(Game::FlagsLoadAndSave::doNotLoadAndSave);

  ASSERT_TRUE(
      BANK.getFunds() == FRAUDS.getEarnedByFrauds() and BANK.getFunds() == 0);

  int iterations = 4;

  std::vector<money_t> funds;
  funds.reserve(iterations);
  for (int i = 0; i < iterations; i++) {
    funds.push_back((i + 1) * std::pow(1000, i));
  };

  money_t expectedFunds = 0;

  for (auto f : funds) {
    FRAUDS.earnCheated(f);
    expectedFunds += f;
  }

  EXPECT_FLOAT_EQ(BANK.getFunds(), expectedFunds);
  EXPECT_FLOAT_EQ(BANK.getFunds(), FRAUDS.getEarnedByFrauds());
}

TEST(Frauds, FraudsIncome) {
  Game::createNewInstance(Game::FlagsLoadAndSave::doNotLoadAndSave);

  int iterations = 8;

  std::vector<money_t> incomes;
  incomes.reserve(iterations);
  for (int i = 0; i < iterations; i++) {
    incomes.push_back((i + 1) * std::pow(1000, i));
  };

  money_t expectedFunds = 0;

  for (auto i : incomes) {
    expectedFunds += i;
    FRAUDS.setFraudIncome(i * AnyTicker::tickPerSec);

    GAME.tick();

    EXPECT_FLOAT_EQ(FRAUDS.getEarnedByFrauds(), expectedFunds);
  }

  EXPECT_FLOAT_EQ(BANK.getFunds(), expectedFunds);
  EXPECT_FLOAT_EQ(BANK.getFunds(), FRAUDS.getEarnedByFrauds());
}