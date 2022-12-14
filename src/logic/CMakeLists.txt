# Copyright (c) 2022-2023 yukkute
# This is the FLOSS software released under MIT License: see LICENSE.md
#  
# SPDX-License-Identifier: MIT

find_package(cereal REQUIRED)

file(GLOB moreonigiri_flow_code *.cpp *.hpp)
add_library(moreonigiri_flow SHARED ${moreonigiri_flow_code})

target_link_libraries(moreonigiri_flow PUBLIC cereal::cereal)
target_include_directories(moreonigiri_flow PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


# ********** i18n gettext generation **********

find_package(Intl)
target_link_libraries(moreonigiri_flow PUBLIC ${Intl_LIBRARIES})

file(GLOB ffs "../../lang/*.po")
set(TEXTDOMAIN moreonigiri_flow)
make_directory(${CMAKE_CURRENT_BINARY_DIR}/locale)
foreach(ff IN LISTS ffs)
  get_filename_component(lang ${ff} NAME_WLE)
  make_directory(${CMAKE_CURRENT_BINARY_DIR}/locale/${lang})
  make_directory(${CMAKE_CURRENT_BINARY_DIR}/locale/${lang}/LC_MESSAGES)
  add_custom_command(
    OUTPUT
      ${CMAKE_CURRENT_BINARY_DIR}/locale/${lang}/LC_MESSAGES/${TEXTDOMAIN}.mo
    DEPENDS
      ${ff}
    COMMAND msgfmt -o
      ${CMAKE_CURRENT_BINARY_DIR}/locale/${lang}/LC_MESSAGES/${TEXTDOMAIN}.mo
      ${ff}
  )
  add_custom_target(gen_logic_${lang} ALL DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/locale/${lang}/LC_MESSAGES/${TEXTDOMAIN}.mo
  )
  add_dependencies(moreonigiri_flow gen_logic_${lang})
endforeach()

target_compile_definitions(moreonigiri_flow PUBLIC "LOGIC_TEXTDOMAIN=\"${TEXTDOMAIN}\"")
target_compile_definitions(moreonigiri_flow PUBLIC "LOGIC_TEXTDIR=\"${CMAKE_CURRENT_BINARY_DIR}/locale\"")


# ********** code quality check **********
if (CODE_QUALITY)
  add_compile_options(-Wall -Wextra -pedantic)
  set_target_properties(
    moreonigiri_flow
    PROPERTIES
    CXX_CLANG_TIDY "clang-tidy"
  )
  # clang-tidy generates warnings in mocs_compilation.cpp
  # you can ignore them for now, as we are finding a way to
  # disable warnings for it
endif()

# ********** verbose save **********
if(VERBOSE_SAVE)
  target_compile_definitions(moreonigiri_flow PUBLIC "VERBOSE_SAVE=true")
else()
  target_compile_definitions(moreonigiri_flow PUBLIC "VERBOSE_SAVE=false")
endif()

# ********** i18n / locale testing **********
if(LANG_TESTING)
  target_compile_definitions(moreonigiri_flow PUBLIC "LANG_TESTING=true")
else()
  target_compile_definitions(moreonigiri_flow PUBLIC "LANG_TESTING=false")
endif()
