#pragma once

/**
  @file translation.hpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "types.hpp"  // IWYU pragma: export

#include <libintl.h>  // for gettext()
#include <memory>     // for unique_ptr translation
#include <vector>     // for translations

#define TRANSLATION                                          \
  struct Translation;                                        \
  inline std::unique_ptr<Translation> translation = nullptr; \
  struct Translation : AnyTranslation

#define TR_DECL(VAR, GETTEXT_NAME) name_t VAR = _(GETTEXT_NAME)
#define _(STRING)                  gettext(STRING)

struct AnyTranslation
{
};

#define TR_INIT(DATA_NAMESPACE) \
  DATA_NAMESPACE::translation = std::make_unique<DATA_NAMESPACE::Translation>()

#define TR_GET(DATA_NAMESPACE, VAR) DATA_NAMESPACE::translation->VAR