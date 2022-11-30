#pragma once

/**
  @file enterprise_data.hpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "translation.hpp"

#include <array>  // for std::array
#include <cmath>  // for std::pow()

namespace EnterpriseData {

constexpr index_t implemented = 7;

constexpr float next = 1.20;

const std::array<index_t, implemented> costMults
    // N+1 enterprise is N times more expensive
    {100, 9, 8, 6, 11, 7, 7};

const std::array<index_t, implemented> incomeMults
    // N+1 enterprise gives N times more income
    {1, 8, 5, 9, 7, 4, 7};

constexpr money_t generateCostIndex(index_t index) {
  money_t cost = 1.0;

  for (index_t i = 0; i <= index; i++) { cost *= costMults.at(i); }

  return cost;
}

constexpr money_t generateIncomeIndex(index_t index) {
  money_t income = 1.0;

  for (index_t i = 0; i <= index; i++) { income *= incomeMults.at(i); }

  return income;
}

constexpr money_t enterpriseCostOfPurchase(
    money_t    baseCost,
    quantity_t baseQuantity,
    quantity_t amount) {
  money_t cost = std::ceil(
      baseCost *
      (std::pow(next, baseQuantity + amount) - std::pow(next, baseQuantity)) /
      (next - 1));

  return cost;
}

// ********** upgradeTier **********

const index_t upgradeTierImplemented = 5;
const index_t upgradeTierTotal =
    upgradeTierImplemented * EnterpriseData::implemented;

constexpr money_t generateUpgradeTierPricetag(index_t n, index_t e) {
  money_t pricetag = (e + 10) * std::pow(10, (2 + e + n));

  return pricetag;
}

TRANSLATION {
  std::array<name_t, implemented> enterpriseNames = {
      {_("e_oniichan"),
       _("e_vending"),
       _("e_farm"),
       _("e_store"),
       _("e_restaurant"),
       _("e_factory"),
       _("e_bank")}};

  std::array<name_t, implemented> enterpriseDescs = {
      {_("e_desc_oniichan"),
       _("e_desc_vending"),
       _("e_desc_farm"),
       _("e_desc_store"),
       _("e_desc_restaurant"),
       _("e_desc_factory"),
       _("e_desc_bank")}};

  name_t enterpriseReport             = _("e_report");
  name_t enterpriseTierMultiplier     = _("e_tier_miltiplier");
  name_t enterpriseIncomePerUnit      = _("e_income_per_unit");
  name_t enterpriseIncomeEnterprise   = _("e_income_enterprise");
  name_t enterprisePercentOfAfk       = _("e_percent_of_afk");
  name_t enterpriseSpentOnEnterprise  = _("e_spent_on_enterprise");
  name_t enterpriseEarnedByEnterprise = _("e_earned_by_enterprise");

  std::array<
      std::array<name_t, upgradeTierImplemented>,
      EnterpriseData::implemented>
      enterpriseTierUpgradeNames{{
          {_("e00u00_aki"),                  // e00 u00
           _("e00u01_himiko"),               // e00 u01
           _("e00u02_yui"),                  // e00 u02
           _("e00u03_ichika"),               // e00 u03
           _("e00u04_megumin")},             // e00 u04
                                             //
          {_("e01u00_orange_juice"),         // e01 u00
           _("e01u01_syrup"),                // e01 u01
           _("e01u02_lime"),                 // e01 u02
           _("e01u03_cola"),                 // e01 u03
           _("e01u04_coconut_milk")},        // e01 u04
                                             //
          {_("e02u00_rice"),                 // e02 u00
           _("e02u01_wheat"),                // e02 u01
           _("e02u02_tomato"),               // e02 u02
           _("e02u03_cucumber"),             // e02 u03
           _("e02u04_sweet_potato")},        // e02 u04
                                             //
          {_("e03u00_open_showcases"),       // e03 u00
           _("e03u01_local_baking"),         // e03 u01
           _("e03u02_obscure_layout"),       // e03 u02
           _("e03u03_no_day_offs"),          // e03 u03
           _("e03u04_loyalty_discounts")},   // e03 u04
                                             //
          {_("e04u00_double_frying"),        // e04 u00
           _("e04u01_triple_frying"),        // e04 u01
           _("e04u02_faster_cooking"),       // e04 u02
           _("e04u03_parallel_cooking"),     // e04 u03
           _("e04u04_cooking_heuristics")},  // e04 u04
                                             //
          {_("e05u00_titanium_tools"),       // e05 u00
           _("e05u01_extra_lubrication"),    // e05 u01
           _("e05u02_subassembly"),          // e05 u02
           _("e05u03_heavy_industry"),       // e05 u03
           _("e05u04_recycling_line")},      // e05 u04
                                             //
          {_("e06u00_consumer_lends"),       // e06 u00
           _("e06u01_spam_calls"),           // e06 u01
           _("e06u02_intl_transfers"),       // e06 u02
           _("e06u03_exponential_rates"),    // e06 u03
           _("e06u04_factorial_rates")},     // e06 u04
      }};

  name_t tierUpgradeHint = _("e_tier_upgrade_hint");
};

}  // namespace EnterpriseData
