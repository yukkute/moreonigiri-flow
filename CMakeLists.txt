# Copyright (c) 2022-2023 yukkute
# This is the FLOSS software released under MIT License: see LICENSE.md
#  
# SPDX-License-Identifier: MIT

cmake_policy(SET CMP0048 NEW)
project(onigiri VERSION 1 LANGUAGES CXX)

cmake_minimum_required(VERSION 3.24.3)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

include(CMakeDependentOption)

option(BUILD_DOC    "Build documentation"                         OFF )
option(BUILD_TESTS  "Build tests"                                 ON  )

option(CODE_QUALITY "Perform pedantic build and clang-tidy check" OFF )
option(VERBOSE_SAVE "Save game to human-readable json"            OFF )
option(LANG_TESTING "Use source-predetermined locale"             OFF )

if(BUILD_DOC)
  add_subdirectory(doc)
endif()



add_subdirectory(src)
