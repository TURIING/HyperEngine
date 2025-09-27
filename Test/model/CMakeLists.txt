file(GLOB_RECURSE SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h" "*.cpp" "*.hpp")

add_executable(Model ${SOURCE_FILES})

target_link_libraries(Model PRIVATE HyperEngine)