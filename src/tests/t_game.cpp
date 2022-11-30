/**
  @file t_game.cpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "game.hpp"

#include <gtest/gtest.h>

TEST(Game, CreateGame) {
  EXPECT_NO_THROW(
      Game::createNewInstance(Game::FlagsLoadAndSave::doNotLoadAndSave));
}