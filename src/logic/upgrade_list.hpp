#pragma once

/**
  @file upgrade_list.hpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "types.hpp"  //money_t

#include <memory>  // for AnyUpgradeVector
#include <vector>  // for AnyUpgradeVector

struct UpgradeListAPI
{
  virtual void onSorted() {}

  virtual void updateSpentOnUpgrades() {}

 protected:
  UpgradeListAPI()          = default;
  virtual ~UpgradeListAPI() = default;
};

class AnyUpgrade;
class UpgradeList
{
 public:
  using AnyUpgradeVector = std::vector<AnyUpgrade*>;

  enum class SortingType { morphologic, by_pricetag };

  static std::shared_ptr<UpgradeList> construct();

  // ********** accessors **********

  AnyUpgradeVector& getUpgrades() { return listOfAllUpgrades; }
  money_t           getSpentOnUpgrades() const { return spentOnUpgrades; }

  // ********** mutators **********
  void sort();
  void setSortingType(SortingType s) { type = s; }

  void addUpgrade(AnyUpgrade& u);

  void recalculateSpentOnUpgrades();

  void connectAPI(UpgradeListAPI& x) { api.push_back(&x); }

  // ********** private methods **********

 private:
  AnyUpgradeVector listOfAllUpgrades;

  money_t spentOnUpgrades = 0;

  SortingType type = SortingType::by_pricetag;
  void        sortMorphologic();
  void        sortPricetag();

  std::vector<UpgradeListAPI*> api;
};