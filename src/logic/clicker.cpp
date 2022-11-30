/**
  @file clicker.cpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "game.hpp"

std::shared_ptr<Clicker> Clicker::firstTimeCreation() {
  auto clicker = std::make_shared<Clicker>();

  for (index_t i = 0; i < ClickerData::upgradeClickerImplemented; i++)
    clicker->upgrades.at(i) = std::make_shared<UpgradeClicker>();

  return clicker;
}

void Clicker::construct() {
  CLICKER->setFormalId("c");
  UpgradeClicker::construct();
  CLICKER->recalculateClickingPower();
}

void Clicker::click() {
  clicksMade += 1;
  earnedByClick += clickingPower;

  BANK.earn(clickingPower);

  for (auto a : api) {
    a->updateClicksMade();
    a->updateEarnedByClick();
  }
}

void Clicker::onSuccessfulUpgradePurchase() { recalculateClickingPower(); }

void Clicker::recalculateClickingPower() {
  clickingPower = ClickerData::baseClickingPower;

  money_t afkMultiplier = 0;

  for (auto u : upgrades) {
    if (u->isPurchased()) afkMultiplier += u->getUpgradePower();
  }

  if (afkMultiplier != 0) {
    clickingPower += AFK.getIncomeAfk() * afkMultiplier;
  }

  for (auto a : api) a->updateClickingPower();
}