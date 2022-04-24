cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME SimpleGameEngineApp)
message("==== ${PROJECT_NAME} ====")
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} main.cpp)
set_target_properties (${PROJECT_NAME} PROPERTIES FOLDER SGE)
sge_set_target_warning_level(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC SimpleGameEngineRender)
file( COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
