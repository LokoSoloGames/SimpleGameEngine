cmake_minimum_required(VERSION 3.18)

set(CMAKE_TOOLCHAIN_FILE "./externals/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(CMAKE_CXX_STANDARD 14)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(PROJECT_NAME Loko-SimpleGameEngine)
project(${PROJECT_NAME})

find_package(fmt CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(EASTL CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(libpng CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

function(sge_set_target_warning_level target_name)
    if(MSVC)
        target_compile_options(${target_name} PRIVATE /WX)     #warning treated as error
        target_compile_options(${target_name} PRIVATE /W4)     #warning level 4
        target_compile_options(${target_name} PRIVATE /wd4100) #warning C4100: unreferenced formal parameter in function
        target_compile_options(${target_name} PRIVATE /wd4201) #warning C4201: nonstandard extension used: nameless struct/union
        target_compile_options(${target_name} PRIVATE /wd4127) #warning C4127: conditional expression is constant
    else()
        target_compile_options(${target_name} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
endfunction()

add_subdirectory(app)
add_subdirectory(src/core)
#add_subdirectory(src/editor)
add_subdirectory(src/render)
add_subdirectory(src/engine)
add_subdirectory(src/shader_compiler)

set_property (DIRECTORY PROPERTY VS_STARTUP_PROJECT SimpleGameEngineApp)