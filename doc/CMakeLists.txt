# Copyright (c) 2022-2023 yukkute
# This is the FLOSS software released under MIT License: see LICENSE.md
#  
# SPDX-License-Identifier: MIT

find_package(Doxygen)
if(DOXYGEN_FOUND)

  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile" "${CMAKE_CURRENT_BINARY_DIR}/doxygen.cfg" @ONLY)
  add_custom_target(onigiri_doc
    COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/doxygen.cfg"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/.."
    COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif()