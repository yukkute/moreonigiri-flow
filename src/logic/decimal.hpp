#pragma once

/**
  @file decimal.hpp
  @author yukkute (yukkute@protonmail.com)

  @copyright Copyright (c) 2022-2023 yukkute
  @license This is FLOSS software released under MIT License: see LICENSE.md
  \n
  SPDX-License-Identifier: MIT
 */

#include "types.hpp"

#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace Dec {
enum Format { longF, shortF, scientificF, integerF, percentF };
constexpr size_t prefixesImplemented = 12;

const std::array<name_t, prefixesImplemented> prexl{
    {"",
     "thousand",
     "million",
     "billion",
     "trillion",
     "quadrillion",
     "quintillion",
     "sextilliion",
     "septillion",
     "octillion",
     "nonillion",
     "decillion"}};

const std::array<name_t, prefixesImplemented> prexs{
    {"", "k", "M", "B", "T", "Qa", "Qi", "Sx", "Sp", "Oc", "No", "Dc"}};

inline std::string
decimal(money_t val, Format format = shortF, int precision = 2) {
  if (not std::isfinite(val)) return "∞";

  bool addPercentSign = false;

  std::ostringstream stream;
  stream << std::setprecision(precision);

  /* prefix calculation */

  size_t logTenTrue = static_cast<size_t>(std::floor(std::log10(val)));

  if ((format == longF or format == shortF) and logTenTrue < 4)
    format = integerF;  // truncate small values

  else if (format == percentF) {
    addPercentSign = true;

    val *= 100;
    logTenTrue += 2;

    format = shortF;
  }

  size_t logTenReadable = logTenTrue - (logTenTrue % 3);
  size_t prefixIndex    = logTenReadable / 3;

  if (prefixIndex >= prefixesImplemented) format = scientificF;

  /* value writing */

  if (val == 0)
    stream << "0";

  else if (format == scientificF)
    stream << std::scientific << val;

  else {
    stream << std::fixed;

    if (format == integerF) {
      stream << std::setprecision(0);
      stream << val;
    }

    else {
      stream << (val / std::pow(10, logTenReadable));
      stream << " ";

      if (format == shortF) {
        stream << prexs.at(prefixIndex);
      } else {
        stream << prexl.at(prefixIndex);
      }
    }
  }

  if (addPercentSign) stream << "%";
  return stream.str();
}
};  // namespace Dec