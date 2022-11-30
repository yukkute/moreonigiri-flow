#pragma once

/**
  @file clicker.hpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "clicker_data.hpp"     // namespace ClickerData
#include "clicker_upgrade.hpp"  // for UpgradeClickerArray
#include "serialization.hpp"    // macro SERIALIZATION
#include "types.hpp"            // money_t

#include <vector>  // for std::vector api

struct ClickerAPI
{
  virtual void updateClickingPower() {}

  virtual void updateClicksMade() {}
  virtual void updateEarnedByClick() {}

 protected:
  ClickerAPI()          = default;
  virtual ~ClickerAPI() = default;
};

class Clicker : public virtual AnyUpgradeParent
{
  friend class Afk;

 public:
  Clicker() = default;

  static std::shared_ptr<Clicker> firstTimeCreation();

  static void construct();

  index_t getClicksMade() const { return clicksMade; }
  money_t getEarnedByClick() const { return earnedByClick; }
  money_t getClickingPower() const { return clickingPower; }

  UpgradeClickerArray& getUpgrades() { return upgrades; }

  void click();
  void connectAPI(ClickerAPI& x) { api.push_back(&x); }

 private:
  void onSuccessfulUpgradePurchase() final;

  void recalculateClickingPower();

  index_t clicksMade    = 0;
  money_t earnedByClick = 0;
  money_t clickingPower = ClickerData::baseClickingPower;

  UpgradeClickerArray upgrades = {};

  std::vector<ClickerAPI*> api;

  SERIALIZATION {
    SER(clicksMade);
    SER(earnedByClick);
    SER(upgrades);
  }
};