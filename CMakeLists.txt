cmake_minimum_required(VERSION 3.19)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 1)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall -O1 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall -O2")

# Project "Touhou Open Engine"
project(thoe
  VERSION ${VERSION_MAJOR}${VERSION_MINOR}.${VERSION_PATCH}
  DESCRIPTION " ---- --- --- "
  HOMEPAGE_URL "https://github.com/OpenA/thoe"
  LANGUAGES CXX
)

include(CheckTypeSize)
include(CheckIncludeFile)
include(CheckIncludeFileCXX)

set(THOE_SRC
  src/main.cpp
)
set(THOE_H
  ${CMAKE_SOURCE_DIR}
)

option(USE_GLFW "use GLFW renderer instead SDL" OFF)
if(USE_GLFW)
  find_package(
    glfw3 3.3 REQUIRED
  )
  set(RENDR_SRC
    src/ui/glfw.cpp
  )
  set(RENDR_LIB
    glfw
  )
  set(RENDR_H
    ${GLFW3_INCLUDE_DIRS}
  )
  message("-- Found GLFW components: (${GLFW3_VERSION_MAJOR}) ${RENDR_H}")
else()
  find_package(
    SDL2 REQUIRED CONFIG
         REQUIRED COMPONENTS
    SDL2
    SDL2_image
    SDL2_mixer
    SDL2_ttf
  )
  set(RENDR_SRC
    src/ui/sdl.cpp
  )
  set(RENDR_LIB
    SDL2
    SDL2_image
    SDL2_mixer
    SDL2_ttf
  )
  set(RENDR_H
    ${SDL2_INCLUDE_DIRS}
  )
  message("-- Found SDL components: (${SDL2_VERSION}) ${RENDR_H}")
endif()

#step 0: create config.h
configure_file(src/config.h.in config.h)
#step 2: add source files
add_executable(thoe
  ${THOE_SRC} ${RENDR_SRC}
)
#step 1: add include files
include_directories(thoe
  ${THOE_H} ${RENDR_H}
)
#step 3: link libraries
target_link_libraries(thoe PRIVATE
  ${RENDR_LIB}
)
