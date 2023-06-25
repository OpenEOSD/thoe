cmake_minimum_required(VERSION 3.19)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 2)

set(CMAKE_C_COMPILER "cc")
set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_COMPILER "c++")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall -O1 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall -O2")

# Project "Touhou Open Engine"
project(thoe
  VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
  DESCRIPTION " ---- --- --- "
  HOMEPAGE_URL "https://github.com/OpenEOSD/thoe"
  LANGUAGES CXX
)

include(CheckTypeSize)
include(CheckIncludeFile)
include(CheckIncludeFileCXX)

add_subdirectory(src)
