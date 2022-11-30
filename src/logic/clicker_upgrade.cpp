/**
  @file clicker_upgrade.cpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "game.hpp"

void UpgradeClicker::construct() {
  for (int i = 0; auto u : CLICKER->getUpgrades()) {
    u->AnyUpgrade::construct(AnyUpgrade::Type::clicker, CLICKER, i);

    u->upgradePower = 0.036f * (u->getSerialNumber() + 1);

    u->pricetag = ClickerData::generateUpgradeClickerPricetag(i);
    u->updateAffordable();
    u->name = TR_GET(ClickerData, upgradeClickerNames.at(i));

    i++;
  }
}

std::string UpgradeClicker::getHint() const {
  std::string hint;
  hint.append("+");
  hint.append(Dec::decimal(getUpgradePower(), Dec::percentF, 1));
  hint.append(" ");
  hint.append(TR_GET(ClickerData, clickerUpgradeHint));
  return hint;
};