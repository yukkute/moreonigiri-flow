#pragma once

#include "types.hpp"

#include <cassert>
#include <iomanip>
#include <sstream>
#include <string>

inline std::string getStringOfWidth2WithNumber(unsigned short number) {
  assert(number >= 0 and number <= 99);

  std::string s;

  if (number <= 9) { s.append("0"); }

  s.append(std::to_string(number));

  assert(s.length() == 2);

  return s;
}

struct FormalId
{
 public:
  std::string getFormalId() const {
    assert(formalId != "");
    return formalId;
  }

 protected:
  FormalId() = default;
  void setFormalId(name_t id) { formalId = id; }

 private:
  std::string formalId = "";
};