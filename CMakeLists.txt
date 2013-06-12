cmake_minimum_required(VERSION 2.8)

set(project_name tango)

project(${project_name})

set(CMAKE_OSX_ARCHITECTURES i386 CACHE STRING "do not build for 64-bit" FORCE)
set(ARCH i386)

set(MOAI_DEV ../moai-dev) # tangerinagames/moai-dev branch: joystick

set(${project_name}_include
  include
  lib/osx/glfw/include
  ${MOAI_DEV}/src
)

file(GLOB_RECURSE ${project_name}_sources src/*.cpp)

include_directories(${${project_name}_include})

# GLFW
add_library(glfw STATIC IMPORTED)
set_property(TARGET glfw PROPERTY IMPORTED_LOCATION lib/osx/glfw/libglfw.a)

# moai-osx
add_library(moai-osx STATIC IMPORTED)
set_property(TARGET moai-osx PROPERTY IMPORTED_LOCATION lib/osx/moai/libmoai-osx.a)

# moai-osx-zlcore
add_library(moai-osx-zlcore STATIC IMPORTED)
set_property(TARGET moai-osx-zlcore PROPERTY IMPORTED_LOCATION lib/osx/moai/libmoai-osx-zlcore.a)

# moai-osx-untz
add_library(moai-osx-untz STATIC IMPORTED)
set_property(TARGET moai-osx-untz PROPERTY IMPORTED_LOCATION lib/osx/moai/libmoai-osx-untz.a)

# moai-osx-luaext
add_library(moai-osx-luaext STATIC IMPORTED)
set_property(TARGET moai-osx-luaext PROPERTY IMPORTED_LOCATION lib/osx/moai/libmoai-osx-luaext.a)

# moai-osx-3rdparty
add_library(moai-osx-3rdparty STATIC IMPORTED)
set_property(TARGET moai-osx-3rdparty PROPERTY IMPORTED_LOCATION lib/osx/moai/libmoai-osx-3rdparty.a)

# frameworks
find_library(cocoa Cocoa)
find_library(iokit IOKit)
find_library(opengl OpenGL)
find_library(audiotoolbox AudioToolbox)
find_library(audiounit AudioUnit)
find_library(system system)
find_library(crypto crypto)
find_library(ssl ssl)

set(${project_name}_libraries
  glfw
  moai-osx
  moai-osx-zlcore
  moai-osx-untz
  moai-osx-luaext
  moai-osx-3rdparty
  ${cocoa}
  ${iokit}
  ${opengl}
  ${audiotoolbox}
  ${audiounit}
  ${system}
  ${crypto}
  ${ssl}
)

add_definitions(-std=c++11)
add_executable(${project_name} ${${project_name}_sources})
target_link_libraries(${project_name} ${${project_name}_libraries})
