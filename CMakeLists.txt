cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE "./externals/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(CMAKE_CXX_STANDARD 14)

set(PROJECT_NAME Loko-SimpleGameEngine)
project(${PROJECT_NAME})

find_package(fmt CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(EASTL CONFIG REQUIRED)

add_definitions("-DUNICODE")
add_subdirectory(app)
add_subdirectory(src/core)
#add_subdirectory(src/editor)
add_subdirectory(src/render)

set_property (DIRECTORY PROPERTY VS_STARTUP_PROJECT SimpleGameEngineApp)