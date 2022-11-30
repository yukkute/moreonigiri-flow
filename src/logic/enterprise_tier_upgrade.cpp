/**
  @file enterprise_tier_upgrade.cpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "game.hpp"

void UpgradeEnterpriseTier::construct() {
  for (auto e : ENTERPRISES) {
    for (int i = 0; auto u : e->upgrades) {
      u->AnyUpgrade::construct(AnyUpgrade::Type::enterpriseTier, e, i);

      u->pricetag =
          EnterpriseData::generateUpgradeTierPricetag(i, u->getParentNumber());

      u->updateAffordable();

      u->name = TR_GET(
          EnterpriseData,
          enterpriseTierUpgradeNames.at(u->getParentNumber()).at(i));

      i++;
    }
  }
}

std::string UpgradeEnterpriseTier::getHint() const {
  std::string hint;
  hint.append("x");
  hint.append(std::to_string(getSerialNumber() + 2));
  hint.append(" ");
  hint.append(TR_GET(EnterpriseData, tierUpgradeHint));
  hint.append(": ");
  hint.append(TR_GET(EnterpriseData, enterpriseNames.at(getParentNumber())));
  return hint;
};
