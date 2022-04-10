cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME SimpleGameEngineApp)
message("==== ${PROJECT_NAME} ====")
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC SimpleGameEngineEditor)