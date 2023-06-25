

set(thoe_extra_lib thtk)
set(thoe_extra_h)
set(thoe_source
  main.cpp
)

option(USE_GLFW "use GLFW renderer instead SDL" OFF)

if(USE_GLFW)
  find_package(
    glfw3 3.3 REQUIRED
  )
  set(thoe_extra_lib
    ${thoe_extra_lib} glfw
  )
  set(thoe_source
    ${thoe_source} ui/glfw.cpp
  )
  set(thoe_extra_h
    ${thoe_extra_h} ${GLFW3_INCLUDE_DIRS}
  )
  message(STATUS "Found GLFW3: ${GLFW3_VERSION_MAJOR}")
else()
  find_package(
    SDL2 REQUIRED CONFIG
         REQUIRED COMPONENTS
    SDL2
    SDL2_image
    SDL2_mixer
    SDL2_ttf
  )
  set(thoe_source
    ${thoe_source} ui/sdl.cpp
  )
  set(thoe_extra_lib
    ${thoe_extra_lib}
    SDL2
    SDL2_image
    SDL2_mixer
    SDL2_ttf
  )
  set(thoe_extra_h
    ${thoe_extra_h} ${SDL2_INCLUDE_DIRS}
  )
  message(STATUS "Found SDL2: ${SDL2_VERSION}")
endif()

#step 0: add submodules
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thtk)
add_subdirectory(thtk)
#step 1: create config.h
configure_file(config.h.in config.h)
#step 2: add source files
add_executable(thoe ${thoe_source})
#step 3: add include files
include_directories(thoe ${thoe_extra_h})
#step 4: link libraries
target_link_libraries(thoe PUBLIC ${thoe_extra_lib})
