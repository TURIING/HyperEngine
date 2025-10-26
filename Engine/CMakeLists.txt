set(HYPER_GPU_PATH external/HyperGpu)

set(EXTERNAL ${CMAKE_CURRENT_SOURCE_DIR}/../external)
include_directories(${HYPER_GPU_PATH}/include ${EXTERNAL}/)
add_subdirectory(${EXTERNAL}/HyperGpu HyperGpu)
add_subdirectory(${EXTERNAL}/assimp assimp)
add_subdirectory(${EXTERNAL}/physfs physfs)
add_subdirectory(${EXTERNAL}/tinyobj tinyobjloader)

file(GLOB_RECURSE SOURCE RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h" "*.cpp" "*.hpp")
add_library(${PROJECT_NAME} SHARED ${SOURCE})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        # Generated headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${EXTERNAL}/glm>
        $<BUILD_INTERFACE:${EXTERNAL}/rocket>
        $<BUILD_INTERFACE:${EXTERNAL}/physfs>
        $<BUILD_INTERFACE:${EXTERNAL}/tinyobj>
)
target_link_libraries(${PROJECT_NAME} PUBLIC HyperGpu assimp physfs tinyobjloader)
set_target_properties(${PROJECT_NAME} PROPERTIES
        XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "YES"
        XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Automatic"
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Apple Development"
        #        XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ""
        XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY "YES"
)

include(GenerateExportHeader)
generate_export_header(
        ${PROJECT_NAME}
        EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/Export.hpp"
        EXPORT_MACRO_NAME "ENGINE_EXPORT"
        NO_EXPORT_MACRO_NAME "ENGINE_NO_EXPORT"
        DEPRECATED_MACRO_NAME "ENGINE_DEPRECATED"
)

configure_file(../Config.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/include/Config.hpp" @ONLY)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_definitions(-DOPENGL_DESKTOP)
else ()
    add_definitions(-DOPENGL_MOBILE)
endif ()
