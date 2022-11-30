#pragma once

/**
  @file enterprise_tier_upgrade.hpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "enterprise_data.hpp"
#include "types.hpp"
#include "upgrade.hpp"

#include <array>   // for UpgradeEnterpriseTierArray
#include <memory>  // for std::shared_ptr<UpgradeEnterpriseTier>

class Enterprise;
class UpgradeEnterpriseTier final : public virtual AnyUpgrade
{
 public:
  UpgradeEnterpriseTier() = default;

  static void construct();

  name_t  getName() const final { return name; }
  money_t getPricetag() const final { return pricetag; };

  std::string getHint() const final;

 private:
  name_t name;

  money_t pricetag;
};

using UpgradeEnterpriseTierArray = std::array<
    std::shared_ptr<UpgradeEnterpriseTier>,
    EnterpriseData::upgradeTierImplemented>;