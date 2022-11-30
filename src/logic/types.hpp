#pragma once

/**
  @file types.hpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include <cstddef>      // for size_t
#include <limits>       // for infinity
#include <string_view>  // for std::string_view

using index_t    = std::size_t;
using quantity_t = index_t;
using money_t    = double;
using name_t     = std::string_view;