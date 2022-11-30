/**
  @file bank.cpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "game.hpp"

// ********** construction **********

std::shared_ptr<Bank> Bank::firstTimeCreation() {
  auto bank = std::make_shared<Bank>();
  return bank;
}

// ********** mutators **********

void Bank::earn(money_t amount) {
  if (amount <= 0) return;

  funds += amount;

  recalculateFundsEarned();

  onBankChanged();
}

bool Bank::spend(money_t amount) {
  if (amount > funds or amount <= 0) return false;

  funds -= amount;

  onBankChanged();
  return true;
}

void Bank::flush() {
  funds = 0;
  onBankChanged();
}

// ********** private methods **********

void Bank::recalculateFundsEarned() {
  fundsEarned = 0;
  fundsEarned += CLICKER->getEarnedByClick();
  fundsEarned += AFK.getEarnedByAfk();
  fundsEarned += FRAUDS.getEarnedByFrauds();
}

void Bank::recalculateFundsSpent() {
  fundsSpent = 0;
  fundsSpent += AFK.getInvestedInAfk();
  fundsSpent += UPGRADELIST.getSpentOnUpgrades();
}

void Bank::onBankChanged() {
  for (auto mm : multimerchSub) mm->onBankChanged();

  for (auto a : api) a->updateFunds();

  updateAffordable();
}

void Bank::updateAffordable() {
  for (auto m : affordableSub) m->updateAffordable();
}
