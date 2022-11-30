/**
  @file merch.cpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "merch.hpp"

#include "game.hpp"

#include <stdexcept>

// =============================
//  MERCH
// =============================

bool AnyMerch::isAffordable() const {
  if (FRAUDS.isGratisPurchase()) return true;
  return affordable;
}

AnyMerch::AnyMerch() { BANK.subscribeMerch(*this); }

void AnyMerch::updateAffordable() {
  affordable = BANK.getFunds() >= getPricetag();

  for (auto a : api) a->updateAffordable();
}

bool AnyMerch::buy() {
  if (not isAvailableForPurchase()) return false;

  if (FRAUDS.isGratisPurchase()) {
    onSuccessfulMerchPurchase();
    return true;
  }

  money_t storePricetag = getPricetag();
  onSuccessfulMerchPurchase();

  bool successfulPurchase = BANK.spend(storePricetag);
  if (not successfulPurchase) throw std::logic_error("merch: could not spend");

  return true;
}

// =============================
//  SINGLEMERCH
// =============================

bool AnySingleMerch::isAvailableForPurchase() const {
  return not isPurchased() and isAffordable();
}

void AnySingleMerch::onSuccessfulMerchPurchase() {
  purchased = true;
  for (auto a : api) a->updatePurchased();

  onSuccessfulSingleMerchPurchase();
}

// =============================
//  MULTIMERCH
// =============================

money_t AnyMultiMerch::getPricetag() const {
  if (doMaxoutPurchase and not FRAUDS.isGratisPurchase()) {
    // cannot do Maxout purchase on free
    return maxoutPricetag;
  }

  return cumulativePricetag;
}

bool AnyMultiMerch::isAvailableForPurchase() const { return isAffordable(); }

void AnyMultiMerch::setCumulative(quantity_t cumulative) {
  cumulativePurchaseAmount = cumulative;
  onCumulativeMaxChanged();
}

void AnyMultiMerch::enableMaxout() {
  doMaxoutPurchase = true;
  onCumulativeMaxChanged();
}

void AnyMultiMerch::disableMaxout() {
  doMaxoutPurchase = false;
  onCumulativeMaxChanged();
}

void AnyMultiMerch::onBankChanged() {
  if (doMaxoutPurchase) { recalculateForMaxout(); }

  updateAffordable();
}

AnyMultiMerch::AnyMultiMerch() { BANK.subscribeMultimerch(*this); }

void AnyMultiMerch::recalculateForMaxout() {
  recalculateMaxoutQuantity();
  recalculateMaxoutPricetag();

  for (auto a : api) {
    a->updateMaxoutQuantity();
    a->updatePricetag();
  }
}

void AnyMultiMerch::recalculateForCumulative() {
  recalculateCumulativePricetag();
  for (auto a : api) { a->updatePricetag(); }
}

void AnyMultiMerch::onSuccessfulMerchPurchase() {
  quantity += (doMaxoutPurchase ? maxQuantity : cumulativePurchaseAmount);
  for (auto a : api) a->updateQuantity();

  onSuccessfulMultiMerchPurchase();

  recalculateForCumulative();
  // is called because quantity changed

  // recalculateForMaxout and updateAffordable
  // will be called from onBankChanged
  // because funds are not spent yet
}

void AnyMultiMerch::onCumulativeMaxChanged() {
  for (auto mm : BANK.getMultimerchSub()) {
    mm->recalculateCumulativePricetag();
    mm->recalculateForMaxout();

    // update both cumulativePricetag and maxoutPricetag
    // because of MultiMerch controls change

    mm->updateAffordable();
    // because cumulative could be increased/decreased
  }
}