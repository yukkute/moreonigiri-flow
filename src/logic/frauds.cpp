/**
  @file frauds.cpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "frauds.hpp"

#include "game.hpp"
#include "merch.hpp"

// ********** construction **********

std::shared_ptr<Frauds> Frauds::construct() {
  auto frauds = std::make_shared<Frauds>();
  return frauds;
}

// ********** mutators **********

void Frauds::earnCheated(money_t amount) {
  fundsCheated += (amount);
  BANK.earn(amount);

  for (auto a : api) a->updateEarnedByFrauds();
}

void Frauds::setFraudIncome(money_t i) {
  fraudIncome = i;
  for (auto a : api) a->updateFraudIncome();
}

void Frauds::resetFraudIncome() { setFraudIncome(0.0); }

// ********** private interfaces **********

void Frauds::tick() {
  if (fraudIncome != 0) earnCheated(fraudIncome / tickPerSec);
}

void Frauds::setGratisPurchase(bool s) {
  gratisPurchase = s;

  AnyMultiMerch::disableMaxout();  // use cumulative purchase
  BANK.onBankChanged();

  for (auto a : api) a->updateGratisPurchase();
}