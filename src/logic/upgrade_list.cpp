/**
  @file upgrade_list.cpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "game.hpp"

#include <algorithm>  // for std::sort()

std::shared_ptr<UpgradeList> UpgradeList::construct() {
  auto upgradelist = std::make_shared<UpgradeList>();
  return upgradelist;
}

void UpgradeList::addUpgrade(AnyUpgrade& u) { listOfAllUpgrades.push_back(&u); }

void UpgradeList::recalculateSpentOnUpgrades() {
  spentOnUpgrades = 0;
  for (auto u : listOfAllUpgrades) {
    if (u->isPurchased()) { spentOnUpgrades += u->getPricetag(); }
  }

  for (auto a : api) a->updateSpentOnUpgrades();

  BANK.recalculateFundsSpent();
}

bool purchaseCompare(const AnyUpgrade& lhs, const AnyUpgrade& rhs) {
  return lhs.isPurchased() < rhs.isPurchased();
}

bool pricetagCompare(const AnyUpgrade& lhs, const AnyUpgrade& rhs) {
  return lhs.getPricetag() < rhs.getPricetag();
}

bool morphologicCompare(const AnyUpgrade& lhs, const AnyUpgrade& rhs) {
  // first  sort by type of upgrade
  //  then  sort by parent index
  //  then  sort by index

  if (lhs.getType() != rhs.getType()) { return lhs.getType() < rhs.getType(); }
  if (lhs.getParentNumber() != rhs.getParentNumber()) {
    return lhs.getParentNumber() < rhs.getParentNumber();
  }
  return lhs.getSerialNumber() < rhs.getSerialNumber();
}

void UpgradeList::sort() {
  switch (type) {
    case SortingType::morphologic: sortMorphologic(); break;
    case SortingType::by_pricetag: sortPricetag(); break;
  };

  for (auto a : api) a->onSorted();
}

void UpgradeList::sortMorphologic() {
  std::sort(
      listOfAllUpgrades.begin(),
      listOfAllUpgrades.end(),
      [](auto lhs, auto rhs) {
        if (lhs->isPurchased() != rhs->isPurchased()) {
          return purchaseCompare(*lhs, *rhs);
        }
        return morphologicCompare(*lhs, *rhs);
      });
}
void UpgradeList::sortPricetag() {
  std::sort(
      listOfAllUpgrades.begin(),
      listOfAllUpgrades.end(),
      [](auto lhs, auto rhs) {
        if (lhs->isPurchased() != rhs->isPurchased()) {
          return purchaseCompare(*lhs, *rhs);
        }
        if (lhs->getPricetag() != rhs->getPricetag()) {
          return pricetagCompare(*lhs, *rhs);
        }
        return morphologicCompare(*lhs, *rhs);
      });
}