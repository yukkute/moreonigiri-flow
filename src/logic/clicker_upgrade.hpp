#pragma once

/**
  @file clicker_upgrade.hpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "clicker_data.hpp"  // namespace ClickerData
#include "types.hpp"
#include "upgrade.hpp"  // class AnyUpdgrade

#include <array>   // for UpgradeClickerArray
#include <memory>  // for std::shared_ptr<UpgradeClicker>

class UpgradeClicker final : public virtual AnyUpgrade
{
 public:
  // ********** constructors **********
  UpgradeClicker() = default;
  static void construct();

  // ********** accessors **********

  name_t      getName() const final { return name; }
  money_t     getPricetag() const final { return pricetag; };
  std::string getHint() const final;

  money_t getUpgradePower() const { return upgradePower; }

 private:
  name_t  name;
  money_t pricetag;

  money_t upgradePower = 0;
};

using UpgradeClickerArray = std::array<
    std::shared_ptr<UpgradeClicker>,
    ClickerData::upgradeClickerImplemented>;