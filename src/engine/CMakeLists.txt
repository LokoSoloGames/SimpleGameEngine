cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME SimpleGameEngineEngine)
message("==== ${PROJECT_NAME} ====")
project(${PROJECT_NAME})

file(GLOB_RECURSE src_files "${CMAKE_CURRENT_SOURCE_DIR}/*.*")
foreach(FILE ${src_files})
    get_filename_component(PARENT_DIR "${FILE}" PATH)
    file(RELATIVE_PATH PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} ${PARENT_DIR})
    string(REPLACE "/" "\\" GROUP "${PARENT_DIR}")
    set(GROUP "${GROUP}")
    source_group("${GROUP}" FILES "${FILE}")
endforeach()
add_library(${PROJECT_NAME} ${src_files})
set_target_properties (${PROJECT_NAME} PROPERTIES FOLDER SGE/Engine)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC SimpleGameEngineRender)
sge_set_target_warning_level(${PROJECT_NAME})