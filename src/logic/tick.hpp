#pragma once

/**
  @file tick.hpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

struct AnyTicker
{
  constexpr static float tickPerSec = 60;
  virtual void           tick() {}

 protected:
  virtual ~AnyTicker() = default;
};