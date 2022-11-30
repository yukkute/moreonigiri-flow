#pragma once

/**
  @file enterprise.hpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "enterprise_data.hpp"          // IWYU pragma: export
#include "enterprise_tier_upgrade.hpp"  // for UpgradeEnterpriseTierArray
#include "formal_id.hpp"                // struct FormalId
#include "merch.hpp"                    // class AnyMultimerch
#include "serialization.hpp"            // macro SERIALIZATION
#include "tick.hpp"                     // interface AnyTicker
#include "types.hpp"                    // money_t

/** @struct EnterpriseAPI enterprise.hpp
    @brief External interface for Enterprise objects
    @see Enterprise#connectAPI
*/
struct EnterpriseAPI : virtual AnyMultiMerchAPI
{
  virtual void updateIncomePerUnit() {}
  virtual void updateIncomeEnterprise() {}
  virtual void updatePercentOfAfk() {}
  virtual void updateSpentOnEnterprise() {}
  virtual void updateEarnedByEnterprise() {}

 protected:
  EnterpriseAPI()          = default;
  virtual ~EnterpriseAPI() = default;
};

/** @class Enterprise
    @brief Agent for passive money making
    @details
    Enterprise has a number of units, each of them producing money. Units can be
   purchased.
*/
class Enterprise final :
    public virtual AnyMultiMerch,
    public virtual AnyUpgradeParent,
    public virtual AnyTicker
{
  // ********** friends **********

  friend class Afk;  // for calling recalculates

  // ********** construction **********
  Enterprise() = default;

 public:
  static std::shared_ptr<Enterprise> firstTimeCreation();

  static void construct();

  // ********** accessors **********

  index_t getIndex() const {
    assert(index >= 0 and index < EnterpriseData::implemented);
    return index;
  }

  index_t getUpgradeParentIndex() const final { return getIndex(); }

  money_t getIncomePerUnit() const { return incomePerUnit; }
  money_t getIncomeEnterprise() const { return incomeEnterprise; }
  float   getPercentOfAfk() const { return percentOfAfk; }
  money_t getSpentOnEnterprise() const { return spentOnEnterprise; }
  money_t getEarnedByEnterprise() const { return earnedByEnterprise; }
  money_t getTierMultiplier() const { return tierMultiplier; }

  UpgradeEnterpriseTierArray upgrades;

  // ********** mutators **********

  void connectAPI(EnterpriseAPI& x) {
    this->api.push_back(&x);
    this->AnyMultiMerch::connectAPI(x);
  }

  // ********** private fields **********

 private:
  index_t index = -1;

  money_t costIndex   = 0;
  money_t costPerUnit = 0;
  money_t costNext    = 0;  // updated with purchase, needed for maxout

  money_t incomeIndex      = 0;
  money_t incomePerUnit    = 0;
  money_t incomeEnterprise = 0;

  float percentOfAfk = 0;

  money_t spentOnEnterprise  = 0;
  money_t earnedByEnterprise = 0;

  money_t tierMultiplier = 1;

  // ********** private interfaces **********

  void recalculateMaxoutQuantity() final;
  void recalculateMaxoutPricetag() final;
  void recalculateCumulativePricetag() final;

  void onSuccessfulMultiMerchPurchase() final;
  void onSuccessfulUpgradePurchase() final;

  void tick() final;

  // ********** private methods  **********

  void recalculateTier();
  void recalculateCostPerUnit();
  void recalculateCostNext();
  void recalculateIncomePerUnit();
  void recalculateIncomeEnterprise();
  void recalculatePercentOfAfk();

  std::vector<EnterpriseAPI*> api;

  SERIALIZATION {
    SER_NAME("multimerch", cereal::virtual_base_class<AnyMultiMerch>(this));
    SER(upgrades);
    SER(spentOnEnterprise);
    SER(earnedByEnterprise);
  }
};