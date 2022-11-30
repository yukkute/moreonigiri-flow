/**
  @file afk.cpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "game.hpp"

// ********** construction **********

std::shared_ptr<Afk> Afk::firstTimeCreation() {
  auto afk = std::make_shared<Afk>();

  for (size_t i = 0; i < afk->enterprises.size(); i++) {
    afk->enterprises.at(i) = Enterprise::firstTimeCreation();
  }

  return afk;
}

void Afk::construct() {
  Enterprise::construct();

  AFK.constructed = true;
  AFK.recalculateIncomeAfk();

  AFK.recalculateInvestedInAfk();
  AFK.recalculateEarnedByAfk();
}

// ********** mutators **********

// ********** private interfaces **********

void Afk::tick() {
  for (auto e : enterprises) { e->tick(); }

  BANK.earn(incomeAfk / tickPerSec);

  recalculateEarnedByAfk();
}

// ********** private methods **********

void Afk::recalculateIncomeAfk() {
  if (not constructed) return;

  incomeAfk = 0;

  for (auto e : enterprises) {
    if (e->getQuantity() == 0) continue;

    incomeAfk += e->getIncomeEnterprise();
  }

  CLICKER->recalculateClickingPower();

  for (auto a : api) a->updateIncomeAfk();  // show updated incomeAfk

  for (auto e : enterprises) { e->recalculatePercentOfAfk(); }
}

void Afk::recalculateInvestedInAfk() {
  if (not constructed) return;

  investedInAfk = 0;

  for (auto e : enterprises) { investedInAfk += e->getSpentOnEnterprise(); }

  BANK.recalculateFundsSpent();

  for (auto a : api) a->updateInvestedInAfk();
}

void Afk::recalculateEarnedByAfk() {
  earnedByAfk = 0;

  for (auto e : enterprises) { earnedByAfk += e->getEarnedByEnterprise(); }

  for (auto a : api) a->updateEarnedByAfk();
}