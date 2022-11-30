#pragma once

/**
  @file afk.hpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "afk_data.hpp"         // IWYU pragma: export
#include "enterprise_data.hpp"  // namespace EnterpriseData
#include "serialization.hpp"    // macro SERIALIZATION
#include "tick.hpp"             // interface AnyTicker
#include "types.hpp"            // money_t

#include <array>   // for EnterprisesArray
#include <memory>  // for std::shared_ptr
#include <vector>  // for std::vector<AfkAPI>

class Enterprise;

// =============================
//  STRUCT AFK_API
// =============================
/**
  @struct AfkAPI
  @brief External interface for Afk singleton
  @see Afk#connectAPI
 */
struct AfkAPI
{
  virtual void updateIncomeAfk() {}

  virtual void updateInvestedInAfk() {}
  virtual void updateEarnedByAfk() {}

 protected:
  AfkAPI()          = default;
  virtual ~AfkAPI() = default;
};

// =============================
//  CLASS AFK
// =============================
/**
  @class Afk
  @brief Enterprise storage and logic
  @details
  Singleton class for storing Enterprise objects and some corresponding data
 */
class Afk : public virtual AnyTicker
{
  // =============================
  //  FRIENDS
  // =============================

  friend class Game;  // for tick()
  friend class Enterprise;

  using EnterprisesArray =
      std::array<std::shared_ptr<Enterprise>, EnterpriseData::implemented>;

  // =============================
  //  CONSTRUCTION
  // =============================

 public:
  Afk(money_t earnedByAfk = 0,
      std::array<std::shared_ptr<Enterprise>, EnterpriseData::implemented>
          enterprises = {}) :
      earnedByAfk(earnedByAfk), enterprises(std::move(enterprises)) {}

  static std::shared_ptr<Afk> firstTimeCreation();

  static void construct();
  // will be called after Game singleton construction,
  // outside of constructor due to Enterprise member functions
  // depend on AFK pointer

  // =============================
  //  ACCESSORS
  // =============================

  /**
    @brief Get incomeAfk
    @details
    Get the combined income of each Enterprise, aka total Afk income
    @return money_t incomeAfk
   */
  money_t getIncomeAfk() const { return incomeAfk; }

  /**
    @brief Get earnedByAfk
    @details
    Get the total money earned by Afk in this save
    @return money_t earnedByAfk
   */
  money_t getEarnedByAfk() const { return earnedByAfk; }

  money_t getInvestedInAfk() const { return investedInAfk; }

  /**
    @brief Access Enterprise array
    @details
    Container for all Enterprise objects. Each Enterprise can be accessed as in
    STL container (std::array)
    @see EnterprisesArray.at(size_t)
    @return EnterprisesArray
   */
  EnterprisesArray getEnterprises() { return enterprises; }

  // =============================
  //  MUTATORS
  // =============================

  void connectAPI(AfkAPI& x) { api.push_back(&x); }

  // =============================
  //  PRIVATE FIELDS
  // =============================

 private:
  bool constructed = false;

  EnterprisesArray enterprises;

  money_t incomeAfk = 0;

  money_t investedInAfk = 0;
  money_t earnedByAfk   = 0;

  std::vector<AfkAPI*> api;

  // =============================
  //  PRIVATE INTERFACES
  // =============================

  void tick() final;

  // =============================
  //  PRIVATE METHODS
  // =============================

  void onCumulativeMaxChanged();

  void recalculateIncomeAfk();

  void recalculateInvestedInAfk();
  void recalculateEarnedByAfk();

  // =============================
  //  SERIALIZATION
  // =============================

  SERIALIZATION { SER(enterprises); }
};