add_definitions(-DPLATFORM_MACOS)
add_definitions(-DVK_USE_PLATFORM_MACOS_MVK)
set(EXTERNAL ${CMAKE_CURRENT_SOURCE_DIR}/../external)
include_directories(${Vulkan_INCLUDE_DIR} ${EXTERNAL})

foreach(_glfw3_option "GLFW_BUILD_TESTS" "GLFW_BUILD_EXAMPLES" "GLFW_BUILD_DOCS" "GLFW_INSTALL")
    set(${_glfw3_option} OFF CACHE INTERNAL "")
endforeach()

add_subdirectory(${EXTERNAL}/assimp assimp)
add_subdirectory(${EXTERNAL}/physfs physfs)
add_subdirectory(${EXTERNAL}/spdlog spdlog)
add_subdirectory(${EXTERNAL}/volk volk)
add_subdirectory(${EXTERNAL}/tinyobj tinyobjloader)
add_subdirectory(${EXTERNAL}/vma vma)
add_subdirectory(${EXTERNAL}/glfw glfw)

if(NOT glslang_FOUND)
    set(GLSLANG_ENABLE_INSTALL ON)
    set(ENABLE_OPT OFF)
    add_subdirectory(${EXTERNAL}/glslang glslang)
endif ()

find_package(Vulkan REQUIRED)

file(GLOB_RECURSE SOURCE RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h" "*.cpp" "*.hpp" "*.inl")
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
        $<BUILD_INTERFACE:${EXTERNAL}/glfw>
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        assimp
        physfs
        tinyobjloader
        spdlog::spdlog
        volk::volk
        ${Vulkan_LIBRARIES}
        glslang::glslang
        glslang::SPIRV
        glfw
)

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
