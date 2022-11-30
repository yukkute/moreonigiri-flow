/**
  @file t_enterprise.cpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "game.hpp"

#include <gtest/gtest.h>
#include <vector>

TEST(Enterprise, SinglePurchase) {
  Game::createNewInstance(Game::FlagsLoadAndSave::doNotLoadAndSave);

  for (auto e : ENTERPRISES) {
    AnyMultiMerch::disableMaxout();
    AnyMultiMerch::setCumulative(1);

    ASSERT_EQ(e->getQuantity(), 0);

    EXPECT_FALSE(e->buy());

    auto pricetag = e->getPricetag();
    BANK.earn(pricetag);

    EXPECT_TRUE(e->buy());

    EXPECT_EQ(e->getQuantity(), 1);
  }
}

TEST(Enterprise, CumulativePurchase) {
  Game::createNewInstance(Game::FlagsLoadAndSave::doNotLoadAndSave);

  int iterations = 4;

  std::vector<quantity_t> cumulatives;
  cumulatives.reserve(iterations);
  for (int i = 0; i < iterations; i++) {
    cumulatives.push_back(std::pow(10 + i, i));
  }

  AnyMultiMerch::disableMaxout();

  for (auto e : ENTERPRISES) { ASSERT_EQ(e->getQuantity(), 0); }

  quantity_t expectedQuantity = 0;

  for (auto c : cumulatives) {
    AnyMultiMerch::setCumulative(c);

    for (auto e : ENTERPRISES) {
      EXPECT_FALSE(e->buy());

      FRAUDS.earnCheated(e->getPricetag());

      EXPECT_TRUE(e->buy());

      EXPECT_FALSE(e->buy());
    }

    expectedQuantity += c;
  }

  for (auto e : ENTERPRISES) {
    EXPECT_EQ(e->getQuantity(), expectedQuantity);
    EXPECT_TRUE(e->getIncomeEnterprise() != 0);
  }
}

TEST(Enterprise, MaxPurchase) {
  Game::createNewInstance(Game::FlagsLoadAndSave::doNotLoadAndSave);

  AnyMultiMerch::enableMaxout();

  int iterations = 3;

  std::vector<quantity_t> expectedMaxPurchases;
  expectedMaxPurchases.reserve(iterations);
  for (int i = 0; i < iterations; i++) {
    expectedMaxPurchases.push_back(2 * (i + 1) * std::pow(10, i));
  }

  quantity_t currentExpectedPurchase = 0;
  for (auto q : expectedMaxPurchases) {
    for (auto e : ENTERPRISES) {
      ASSERT_EQ(e->getQuantity(), currentExpectedPurchase);

      EXPECT_FALSE(e->buy());

      BANK.earn(EnterpriseData::enterpriseCostOfPurchase(
          EnterpriseData::generateCostIndex(e->getIndex()),
          currentExpectedPurchase,
          q));

      EXPECT_TRUE(e->buy());

      ASSERT_EQ(e->getQuantity(), currentExpectedPurchase + q);

      EXPECT_FALSE(e->buy());
    }

    currentExpectedPurchase += q;
  }
}