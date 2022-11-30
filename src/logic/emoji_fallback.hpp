#pragma once

/**
  @file emoji_fallback.hpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "enterprise_data.hpp"
#include "types.hpp"

constexpr static std::array<name_t, EnterpriseData::implemented>
    enterpriseEmoji{
        "🐱",
        "🥤",
        "🚜",
        "🛒",
        "🍲",
        "🚛",
        "💴",
    };

constexpr static name_t clickerEmoji{"🔟"};