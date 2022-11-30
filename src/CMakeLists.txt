# Copyright (c) 2022-2023 yukkute
# This is the FLOSS software released under MIT License: see LICENSE.md
#  
# SPDX-License-Identifier: MIT

#  +=============================+
#  |       MOREONIGIRI_FLOW      |
#  +=============================+

add_subdirectory(logic)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()
