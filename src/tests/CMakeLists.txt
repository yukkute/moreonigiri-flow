# Copyright (c) 2022-2023 yukkute
# This is the FLOSS software released under MIT License: see LICENSE.md
#
# SPDX-License-Identifier: MIT

find_package(GTest REQUIRED)

enable_testing()

file(GLOB moreonigiri_flow_test_code *.cpp *.hpp)

add_executable(moreonigiri_flow_test ${moreonigiri_flow_test_code})

target_link_libraries(moreonigiri_flow_test moreonigiri_flow)
target_link_libraries(moreonigiri_flow_test GTest::gtest GTest::gtest_main)

gtest_add_tests(TARGET moreonigiri_flow_test)
