#pragma once

/**
  @file frauds.hpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "frauds_data.hpp"    // IWYU pragma: export
#include "serialization.hpp"  // macro SERIALIZATION
#include "tick.hpp"           // interface AnyTicker
#include "types.hpp"          // money_t

#include <memory>  // for FRAUDS
#include <vector>  // for std::vector<FraudsAPI>

struct FraudsAPI
{
  virtual void updateEarnedByFrauds() {}
  virtual void updateFraudIncome() {}

  virtual void updateGratisPurchase() {}
};

class Frauds : public virtual AnyTicker
{
  // ********** friends **********
  friend class Game;  // for tick()

 public:
  // ********** construction **********

  static std::shared_ptr<Frauds> construct();

  // ********** accessors **********
  money_t getEarnedByFrauds() const { return fundsCheated; }
  money_t getFraudIncome() const { return fraudIncome; }

  money_t isGratisPurchase() const { return gratisPurchase; }

  // ********** mutators **********
  void earnCheated(money_t amount);

  void setFraudIncome(money_t i);
  void resetFraudIncome();

  void enableGratisPurchase() { setGratisPurchase(true); }
  void disableGratisPurchase() { setGratisPurchase(false); }

  void connectAPI(FraudsAPI& x) { api.push_back(&x); }

  // ********** private fields **********
 private:
  money_t fundsCheated = 0;
  money_t fraudIncome  = 0;

  bool gratisPurchase = false;

  std::vector<FraudsAPI*> api;

  // ********** private interfaces **********
  void tick() final;

  void setGratisPurchase(bool s);

  SERIALIZATION { SER(fundsCheated); }
};