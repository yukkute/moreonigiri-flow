/**
  @file game.cpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "game.hpp"

#include <stdexcept>  // for std::domain_error

volatile static bool langInitialized = false;
const static char*   locale;

static void langInit() {
  if (LANG_TESTING) {
    locale = setlocale(LC_ALL, "ru_RU.UTF-8");
  } else {
    locale = setlocale(LC_ALL, "");
  }

  // std::cout << locale << "\n";

  bindtextdomain(LOGIC_TEXTDOMAIN, LOGIC_TEXTDIR);
  bind_textdomain_codeset(LOGIC_TEXTDOMAIN, "UTF-8");

  textdomain(LOGIC_TEXTDOMAIN);

  TR_INIT(AfkData);
  TR_INIT(BankData);
  TR_INIT(ClickerData);
  TR_INIT(EnterpriseData);
  TR_INIT(FraudsData);
  TR_INIT(GameData);
  TR_INIT(UpgradeData);

  langInitialized = true;

  // langInit sets the onigiri_logic (liblogic) language
  // as in GNU Gettext
}

namespace Archive {
using JI = cereal::JSONInputArchive;
using JO = cereal::JSONOutputArchive;
using BI = cereal::PortableBinaryInputArchive;
using BO = cereal::PortableBinaryOutputArchive;

template <class IO, class J, class B>
struct Ar
{
  Ar(IO& s) {
    if (VERBOSE_SAVE)
      initJson(s);
    else
      initBinary(s);
  }

  void initJson(IO& s) { json = std::make_unique<J>(s); }
  void initBinary(IO& s) { binary = std::make_unique<B>(s); }

  std::unique_ptr<J> json   = nullptr;
  std::unique_ptr<B> binary = nullptr;

  template <class T>
  inline void operator()(T&& arg) {
    if (VERBOSE_SAVE and json) {
      (*json)(arg);
      return;
    }
    if (binary) {
      (*binary)(arg);
      return;
    }
    throw std::domain_error(
        "Game Archive error: no archive reacted to call operator");
  }

  ~Ar() {
    json.reset();
    binary.reset();
  }
};

using In  = Ar<std::ifstream, JI, BI>;
using Out = Ar<std::ofstream, JO, BO>;

template <>
void Out::initJson(std::ofstream& s) {
  auto e = JO::Options(324, JO::Options::IndentChar::space, 1);
  json   = std::make_unique<JO>(s, e);
}

}  // namespace Archive

void Game::createNewInstance(FlagsLoadAndSave f) {
  if (not langInitialized) langInit();

  Game g               = Game();
  g.whetherLoadAndSave = f;

  auto game     = std::make_shared<Game>(g);
  Game::gameptr = game;

  if (game->whetherLoadAndSave == FlagsLoadAndSave::doLoadAndSave) {
    game->loadGameFromSave();
  } else {
    game->createNewGame();
  }

  game->clicker->construct();  // for clicker upgrades construction
  game->afk->construct();      // for enterprise/afk construction
  game->upgradelist->sort();   // for initial sort of upgrades
}

void Game::createNewGame() {
  bank        = Bank::firstTimeCreation();
  frauds      = Frauds::construct();
  upgradelist = UpgradeList::construct();
  afk         = Afk::firstTimeCreation();
  clicker     = Clicker::firstTimeCreation();
}

void Game::loadGameFromSave() {
  std::ifstream inFile;
  inFile = VERBOSE_SAVE ? std::ifstream("save.json")
                        : std::ifstream("save.onigiri", std::ios::binary);

  if (not inFile) {
    createNewGame();
    return;
  }

  Archive::In in = (inFile);

  upgradelist = UpgradeList::construct();

  in(cereal::make_nvp("game", *this));
}

void Game::save() {
  std::ofstream outFile;
  if (VERBOSE_SAVE) {
    outFile = std::ofstream("save.json");
  } else {
    outFile = std::ofstream("save.onigiri", std::ios::binary);
  }

  Archive::Out out = (outFile);

  out(cereal::make_nvp("game", *this));
}

Game::~Game() {
  if (whetherLoadAndSave == FlagsLoadAndSave::doLoadAndSave) save();
}

Game& Game::getGame() {
  assert(gameptr);
  return *gameptr;
}

Bank& Game::getBank() const {
  assert(bank);
  return *bank;
}

Frauds& Game::getFrauds() const {
  assert(frauds);
  return *frauds;
}

UpgradeList& Game::getUpgradeList() const {
  assert(upgradelist);
  return *upgradelist;
}

Afk& Game::getAfk() const {
  assert(afk);
  return *afk;
}

std::shared_ptr<Clicker> Game::getClicker() const {
  assert(clicker);
  return clicker;
}

void Game::tick() {
  AFK.tick();
  FRAUDS.tick();
}