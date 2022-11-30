/**
  @file upgrade.cpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is the FLOSS software released under MIT License: see LICENSE.md
  \n

  SPDX-License-Identifier: MIT
 */

#include "game.hpp"

void AnyUpgrade::construct(
    Type                t,
    AnyUpgradeParentPtr passParent,
    index_t             passSerialNumber) {
  type         = t;
  parent       = passParent;
  serialNumber = passSerialNumber;

  std::string formalId;
  formalId.append(parent->getFormalId());
  formalId.append("_u");
  formalId.append(getStringOfWidth2WithNumber(serialNumber));

  setFormalId(formalId);

  assert(getFormalId().length() > 4);

  UPGRADELIST.addUpgrade(*this);
}

void AnyUpgrade::onSuccessfulSingleMerchPurchase() {
  parent->onSuccessfulUpgradePurchase();
  UPGRADELIST.sort();
  UPGRADELIST.recalculateSpentOnUpgrades();
}