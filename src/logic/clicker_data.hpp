#pragma once

/**
  @file clicker_data.hpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "translation.hpp"

#include <array>
#include <cmath>

namespace ClickerData {

const int baseClickingPower = 10;

const index_t     upgradeClickerImplemented = 5;
constexpr money_t generateUpgradeClickerPricetag(index_t n) {
  money_t pricetag = (36 + n) * std::pow(100, n + 1);

  return pricetag;
}

TRANSLATION {
  name_t kitchenClickName = _("kitchen_click");
  name_t clickerReport    = _("c_clicker_report");

  name_t clickingPower = _("c_clicking_power");
  name_t clicksMade    = _("c_clicks_made");
  name_t earnedByClick = _("c_earned_by_click");

  std::array<name_t, upgradeClickerImplemented> upgradeClickerNames{{
      _("c_u00_strong_finger"),
      _("c_u01_rapid_finger"),
      _("c_u02_finger_tremble"),
      _("c_u03_light_mouse"),
      _("c_u04_gaming_mouse"),
  }};

  name_t clickerUpgradeHint = _("c_upgrade_hint");
};

}  // namespace ClickerData