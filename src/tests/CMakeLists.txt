# Copyright (c) 2022-2023 yukkute
# This is the FLOSS software released under MIT License: see LICENSE.md
#
# SPDX-License-Identifier: MIT

find_package(GTest REQUIRED)

enable_testing()

file(GLOB onigiri_test_code *.cpp *.hpp)

add_executable(onigiri_test ${onigiri_test_code})

target_link_libraries(onigiri_test onigiri_logic)
target_link_libraries(onigiri_test GTest::gtest GTest::gtest_main)

gtest_add_tests(TARGET onigiri_test)
