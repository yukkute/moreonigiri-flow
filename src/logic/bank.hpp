#pragma once

/**
  @file bank.hpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is the FLOSS software released under MIT License: see LICENSE.md
  \n

  SPDX-License-Identifier: MIT
 */

#include "bank_data.hpp"      // IWYU pragma: export
#include "serialization.hpp"  // macro SERIALIZATION
#include "types.hpp"          // money_t

#include <memory>  // for std::shared_ptr
#include <vector>  // for vector Merch, MultiMerch

struct BankAPI
{
  virtual void updateFunds() {}
};

class AnyMerch;
class AnyMultiMerch;

class Bank
{
  friend class Frauds;

 public:
  // ********** construction **********

  Bank(money_t funds = 0) : funds(funds) {}

  static std::shared_ptr<Bank> firstTimeCreation();

  // ********** accessors **********
  money_t getFunds() const { return funds; }
  money_t getFundsEarnt() const { return fundsEarned; }
  money_t getFundsSpent() const { return fundsSpent; }

  std::vector<AnyMultiMerch*>& getMultimerchSub() { return multimerchSub; }

  // ********** mutators **********
  void earn(money_t amount);
  bool spend(money_t amount);
  void flush();

  void recalculateFundsSpent();

  void connectAPI(BankAPI& x) { api.push_back(&x); }

  // ********** misc methods **********

  void subscribeMerch(AnyMerch& m) { affordableSub.push_back(&m); }
  void subscribeMultimerch(AnyMultiMerch& m) { multimerchSub.push_back(&m); }

  // ********** private fields **********

 private:
  money_t funds       = 0;
  money_t fundsEarned = 0;
  money_t fundsSpent  = 0;

  std::vector<AnyMerch*>      affordableSub;  // for updating Affordable
  std::vector<AnyMultiMerch*> multimerchSub;  // for proper Max purchase

  std::vector<BankAPI*> api;

  // ********** private methods **********

  void recalculateFundsEarned();

  void onBankChanged();
  void updateAffordable();

  SERIALIZATION { SER(funds); }
};