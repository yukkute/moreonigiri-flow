#pragma once

/**
  @file merch.hpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "serialization.hpp"  // macro SERIALIZATION
#include "types.hpp"

#include <vector>

// =============================
//  MERCH
// =============================

struct AnyMerchAPI
{
  virtual void updateAffordable() {}
};

class AnyMerch
{
  /* accessors */
 public:
  virtual money_t getPricetag() const            = 0;
  virtual bool    isAvailableForPurchase() const = 0;

  bool isAffordable() const;

  /* mutators */

  void updateAffordable();
  bool buy();

  void connectAPI(AnyMerchAPI& x) { api.push_back(&x); }

  /* protected interfaces */
 protected:
  AnyMerch();

  /* private interfaces */
 private:
  virtual void onSuccessfulMerchPurchase() = 0;

  /* private fields */
  bool affordable = false;

  std::vector<AnyMerchAPI*> api;
};

// =============================
//  SINGLEMERCH
// =============================

struct AnySingleMerchAPI : virtual AnyMerchAPI
{
  virtual void updatePurchased() {}
};

class AnySingleMerch : public virtual AnyMerch
{
  /* accessors */
 public:
  bool isAvailableForPurchase() const override;

  bool isPurchased() const { return purchased; }

  /* mutators */

  void connectAPI(AnySingleMerchAPI& x) {
    api.push_back(&x);
    AnyMerch::connectAPI(x);
  }

  /* protected interfaces */
 protected:
  AnySingleMerch() = default;
  virtual void onSuccessfulSingleMerchPurchase() {}

  /* private interfaces */
 private:
  void onSuccessfulMerchPurchase() final;

  /* private fields */

  bool purchased = false;

  std::vector<AnySingleMerchAPI*> api;

  SERIALIZATION { SER(purchased); }
};

// =============================
//  MULTIMERCH
// =============================

struct AnyMultiMerchAPI : virtual AnyMerchAPI
{
  virtual void updateQuantity() {}
  virtual void updateMaxoutQuantity() {}
  virtual void updatePricetag() {}
};

class AnyMultiMerch : public virtual AnyMerch
{
 public:
  /* accessors */

  money_t getPricetag() const final;
  bool    isAvailableForPurchase() const override;

  quantity_t getQuantity() const { return quantity; };
  quantity_t getMaxQuantity() const { return maxQuantity; }

  static quantity_t getCumulative() { return cumulativePurchaseAmount; }

  static bool isMaxout() { return doMaxoutPurchase; }

  /* mutators */

  static void setCumulative(quantity_t cumulative);
  static void enableMaxout();
  static void disableMaxout();

  void onBankChanged();

  void connectAPI(AnyMultiMerchAPI& x) {
    api.push_back(&x);
    AnyMerch::connectAPI(x);
  }

  /* protected interfaces */

 protected:
  AnyMultiMerch();

  virtual void recalculateMaxoutQuantity()     = 0;
  virtual void recalculateMaxoutPricetag()     = 0;
  virtual void recalculateCumulativePricetag() = 0;

  virtual void onSuccessfulMultiMerchPurchase() {}

  /* protected fields */

  quantity_t maxQuantity = 0;

  money_t cumulativePricetag = 0;
  money_t maxoutPricetag     = 0;

  /* private interfaces */

 private:
  void onSuccessfulMerchPurchase() final;

  static void onCumulativeMaxChanged();

  void recalculateForMaxout();
  void recalculateForCumulative();

  /* private fields */

  static inline quantity_t cumulativePurchaseAmount = 1;
  static inline bool       doMaxoutPurchase         = false;
  quantity_t               quantity                 = 0;

  std::vector<AnyMultiMerchAPI*> api;

  SERIALIZATION { SER(quantity); }
};