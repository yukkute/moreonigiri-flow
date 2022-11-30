/**
  @file enterprise.cpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "game.hpp"

#include <cmath>  // for formulas

// ********** construction **********

void Enterprise::construct() {
  for (int i = 0; auto e : ENTERPRISES) {
    assert(e);

    e->index = i;
    e->setFormalId("e" + getStringOfWidth2WithNumber(i));

    assert(e->getFormalId().length() == 3);

    i++;
  }

  UpgradeEnterpriseTier::construct();

  for (auto e : ENTERPRISES) {
    e->costIndex   = EnterpriseData::generateCostIndex(e->getIndex()),
    e->incomeIndex = EnterpriseData::generateIncomeIndex(e->getIndex()),

    e->recalculateCostPerUnit();
    e->recalculateTier();

    e->recalculateCumulativePricetag();
    e->recalculateIncomePerUnit();
  }
}

std::shared_ptr<Enterprise> Enterprise::firstTimeCreation() {
  auto e = std::shared_ptr<Enterprise>(new Enterprise());

  for (index_t i = 0; i < EnterpriseData::upgradeTierImplemented; i++)
    e->upgrades.at(i) = std::make_shared<UpgradeEnterpriseTier>();

  return e;
}

// ********** private interfaces **********

void Enterprise::recalculateCumulativePricetag() {
  cumulativePricetag = EnterpriseData::enterpriseCostOfPurchase(
      costPerUnit,
      getQuantity(),
      getCumulative());
}

void Enterprise::recalculateMaxoutQuantity() {
  auto q = getQuantity();
  auto n = EnterpriseData::next;
  auto b = BANK.getFunds();

  if (costNext > b) {
    maxQuantity = 0;
  } else {
    maxQuantity =
        static_cast<quantity_t>(std::floor(
            std::log2(std::pow(n, q) + ((b * (n - 1)) / (costPerUnit))) /
            std::log2(n))) -
        q;
  }
}

void Enterprise::recalculateMaxoutPricetag() {
  money_t pricetag;

  if (maxQuantity == 0 or maxQuantity == 1) {
    pricetag = costNext;
  } else {
    pricetag = EnterpriseData::enterpriseCostOfPurchase(
        costPerUnit,
        getQuantity(),
        getMaxQuantity());
  }

  maxoutPricetag = pricetag;
}

void Enterprise::onSuccessfulMultiMerchPurchase() {
  spentOnEnterprise += getPricetag();
  AFK.recalculateInvestedInAfk();

  for (auto a : api) a->updateSpentOnEnterprise();

  recalculateCostNext();
  recalculateIncomeEnterprise();
}

void Enterprise::onSuccessfulUpgradePurchase() { recalculateTier(); }

void Enterprise::tick() {
  if (getQuantity() == 0) return;
  earnedByEnterprise += (incomeEnterprise / tickPerSec);

  for (auto a : api) a->updateEarnedByEnterprise();
}

// ********** private methods **********

void Enterprise::recalculateTier() {
  this->tierMultiplier = 1;

  for (auto u : upgrades) {
    if (u->isPurchased()) {
      this->tierMultiplier *=
          (u->getSerialNumber() + 2);  // TODO - tier bonus from upgrade
    }
  }
  recalculateIncomePerUnit();
}

void Enterprise::recalculateCostPerUnit() {
  costPerUnit = costIndex;
  recalculateCostNext();
}

void Enterprise::recalculateCostNext() {
  if (getQuantity() == 0) {
    costNext = costPerUnit;
  } else {
    costNext = std::ceil(
        (costPerUnit / (EnterpriseData::next - 1)) *
        (std::pow(EnterpriseData::next, getQuantity() + 1) -
         std::pow(EnterpriseData::next, getQuantity())));
  }
}

void Enterprise::recalculateIncomePerUnit() {
  incomePerUnit = incomeIndex * tierMultiplier;

  for (auto a : api) a->updateIncomePerUnit();

  recalculateIncomeEnterprise();
}

void Enterprise::recalculateIncomeEnterprise() {
  incomeEnterprise = incomePerUnit * getQuantity();

  for (auto a : api) a->updateIncomeEnterprise();

  AFK.recalculateIncomeAfk();
}

void Enterprise::recalculatePercentOfAfk() {
  percentOfAfk =
      AFK.getIncomeAfk() >= 1
          ? static_cast<float>(getIncomeEnterprise() / AFK.getIncomeAfk())
          : 0.0f;

  for (auto a : api) a->updatePercentOfAfk();
}