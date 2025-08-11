cmake_minimum_required(VERSION 3.29)
project(HyperEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(HYPER_GPU_PATH external/HyperGpu)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

include_directories(${HYPER_GPU_PATH}/include external/)
add_subdirectory(external/HyperGpu)
add_subdirectory(external/assimp)

file(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCE})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC HyperGpu assimp)
set_target_properties(${PROJECT_NAME} PROPERTIES
        XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "YES"
        XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Automatic"
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Apple Development"
        #        XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ""
        XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY "YES"
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_definitions(-DOPENGL_DESKTOP)
else ()
    add_definitions(-DOPENGL_MOBILE)
endif ()