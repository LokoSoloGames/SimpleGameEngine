set(PROJECT_NAME SimpleGameEngineApp)
message("==== ${PROJECT_NAME} ====")
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} main.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD ON UNITY_BUILD_MODE BATCH UNITY_BUILD_BATCH_SIZE 32)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER SGE)
sge_set_target_warning_level(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC SimpleGameEngineRender)
file( COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

add_dependencies(${PROJECT_NAME} SimpleGameEngineShaderCompiler)
