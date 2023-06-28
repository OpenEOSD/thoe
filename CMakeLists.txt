cmake_minimum_required(VERSION 3.19)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 3)

set(CMAKE_C_COMPILER "cc")
set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_COMPILER "c++")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall -O1 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall -O2")

# Project "Touhou Open Engine"
project(thoe
  VERSION ${VERSION_MAJOR}.${VERSION_MINOR}${VERSION_PATCH}
  DESCRIPTION " ---- --- --- "
  HOMEPAGE_URL "https://github.com/OpenEOSD/thoe"
  LANGUAGES CXX
)

include(CheckTypeSize)
include(CheckIncludeFile)
include(CheckIncludeFileCXX)

option(USE_Hg "use Mercurial versioning repo instead Git" OFF)

if(USE_Hg)
  find_package(Hg)
else()
  find_package(Git)

  if(Git_FOUND)
    execute_process(
      COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_DESCRIBE_VERSION
      RESULT_VARIABLE GIT_DESCRIBE_ERROR_CODE
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(NOT GIT_DESCRIBE_ERROR_CODE)
      set(VERSION_BUILD "git-${GIT_DESCRIBE_VERSION}")
    endif()
  endif()
endif()

if(NOT DEFINED VERSION_BUILD)
  set(VERSION_BUILD "unknown")
  message(WARNING "Failed to determine VERSION_BUILD from Git tags. Using default version \"${VERSION_BUILD}\".")
endif()

add_subdirectory(src)
