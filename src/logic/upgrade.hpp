#pragma once

/**
  @file upgrade.hpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "formal_id.hpp"     // struct FormalId
#include "merch.hpp"         // class AnyMerch
#include "types.hpp"         // index_t
#include "upgrade_data.hpp"  // IWYU pragma: export

#include <vector>  // for std::vector api

struct AnyUpgradeAPI : AnySingleMerchAPI
{
};

struct AnyUpgradeParent : public virtual FormalId
{
  virtual index_t getUpgradeParentIndex() const { return 0; }
  virtual void    onSuccessfulUpgradePurchase() {}
};

using AnyUpgradeParentPtr = std::shared_ptr<AnyUpgradeParent>;

class AnyUpgrade : public virtual AnySingleMerch, public virtual FormalId
{
 public:
  enum class Type { enterpriseTier, clicker, afkMultiplier };

  virtual name_t      getName() const { return "!NAME"; }
  virtual std::string getHint() const { return "!HINT"; }

  Type    getType() const { return type; }
  index_t getSerialNumber() const { return serialNumber; }
  index_t getParentNumber() const { return parent->getUpgradeParentIndex(); }

  void connectAPI(AnyUpgradeAPI& x) {
    api.push_back(&x);
    AnySingleMerch::connectAPI(x);
  }

 protected:
  AnyUpgrade() = default;
  void construct(Type, AnyUpgradeParentPtr, index_t);

 private:
  Type                type;
  index_t             serialNumber;
  AnyUpgradeParentPtr parent;

  void onSuccessfulSingleMerchPurchase() final;

  std::vector<AnyUpgradeAPI*> api;
};