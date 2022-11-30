#pragma once

/**
  @file game.hpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

// logic library exports
#include "afk.hpp"            // for AFK
#include "bank.hpp"           // for BANK
#include "clicker.hpp"        // for CLICKER
#include "enterprise.hpp"     // for ENTERPRISES
#include "frauds.hpp"         // for FRAUDS
#include "serialization.hpp"  // macro SERIALIZATION
#include "tick.hpp"           // interface AnyTicker
#include "upgrade_list.hpp"

// indirect exports
#include "decimal.hpp"         // IWYU pragma: export
#include "emoji_fallback.hpp"  // IWYU pragma: export
#include "game_data.hpp"       // IWYU pragma: export

#include <fstream>  // for inFile, outFile
#include <memory>

#define NO_LOAD_SAVE true

#define GAME        Game::getGame()
#define BANK        GAME.getBank()
#define FRAUDS      GAME.getFrauds()
#define UPGRADELIST GAME.getUpgradeList()
#define AFK         GAME.getAfk()
#define ENTERPRISES GAME.getAfk().getEnterprises()
#define CLICKER     GAME.getClicker()

class Game final : public virtual AnyTicker
{
 public:
  enum class FlagsLoadAndSave { doLoadAndSave, doNotLoadAndSave };

  static void createNewInstance(
      FlagsLoadAndSave = FlagsLoadAndSave::doLoadAndSave);

  void save();
  ~Game() final;

  static Game& getGame();

  Bank&                    getBank() const;
  Frauds&                  getFrauds() const;
  UpgradeList&             getUpgradeList() const;
  Afk&                     getAfk() const;
  std::shared_ptr<Clicker> getClicker() const;

  void tick() final;

 private:
  static inline std::shared_ptr<Game> gameptr = nullptr;

  FlagsLoadAndSave whetherLoadAndSave = FlagsLoadAndSave::doLoadAndSave;

  std::shared_ptr<Bank>        bank        = nullptr;
  std::shared_ptr<Frauds>      frauds      = nullptr;
  std::shared_ptr<UpgradeList> upgradelist = nullptr;
  std::shared_ptr<Afk>         afk         = nullptr;
  std::shared_ptr<Clicker>     clicker     = nullptr;

  void createNewGame();
  void loadGameFromSave();

  SERIALIZATION {
    SER(bank);
    SER(frauds);
    SER(afk);
    SER(clicker);
  }
};