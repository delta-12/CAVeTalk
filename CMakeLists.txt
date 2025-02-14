cmake_minimum_required(VERSION 3.30)
project(CavemanController LANGUAGES C CXX)

set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
set(LINK_LAYER_DIR ${LIB_DIR}/link_layer)
set(LINK_LAYER_INC_DIR ${LINK_LAYER_DIR}/inc)
set(LINK_LAYER_SRC_DIR ${LINK_LAYER_DIR}/src)
set(LINK_LAYER_SRCS ${LINK_LAYER_SRC_DIR}/cave_talk_link.c)

################################################################################
# CI and tools
################################################################################
set(TOOLS_DIR ${CMAKE_SOURCE_DIR}/tools)
set(CPPCHECK_SOURCES ${LINK_LAYER_SRCS})
include(${TOOLS_DIR}/cppcheck/cppcheck.cmake)
set(UNCRUSTIFY_SOURCES ${CPPCHECK_SOURCES})
set(UNCRUSTIFY_INC_DIRS ${LINK_LAYER_INC_DIR})
include(${TOOLS_DIR}/uncrustify/uncrustify.cmake)