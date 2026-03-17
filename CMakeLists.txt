cmake_minimum_required(VERSION 3.29)
project(Gomoku)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "Silence CMake < 3.5 errors")
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Release)

if (NOT CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_options(-O3 -g -march=native -ffast-math -DNDEBUG)
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

include(FetchContent)
set(SFML_BUILD_AUDIO ON)
set(SFML_BUILD_NETWORK OFF)
set(SFML_BUILD_WINDOW ON)
set(SFML_BUILD_GRAPHICS ON)
set(SFML_BUILD_SYSTEM ON)

FetchContent_Declare(
        Ogg
        GIT_REPOSITORY https://github.com/xiph/ogg.git
        GIT_TAG v1.3.5
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(Ogg)

if (TARGET ogg AND NOT TARGET Ogg::ogg)
    add_library(Ogg::ogg ALIAS ogg)
endif ()

FetchContent_Declare(
        Vorbis
        GIT_REPOSITORY https://github.com/xiph/vorbis.git
        GIT_TAG v1.3.7
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(Vorbis)
if (TARGET vorbis AND NOT TARGET Vorbis::vorbis)
    add_library(Vorbis::vorbis ALIAS vorbis)
endif ()
if (TARGET vorbisfile AND NOT TARGET Vorbis::vorbisfile)
    add_library(Vorbis::vorbisfile ALIAS vorbisfile)
endif ()
if (TARGET vorbisenc AND NOT TARGET Vorbis::vorbisenc)
    add_library(Vorbis::vorbisenc ALIAS vorbisenc)
endif ()

FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.1
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.4
)
FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)

set(BOARD_FILES
        src/board/Board.cpp
)

set(DISPLAY_FILES
        src/display/components/Button.cpp
        src/display/scenes/BoardScene.cpp
        src/display/scenes/MainMenuScene.cpp
        src/display/scenes/PvEScene.cpp
        src/display/scenes/PvPScene.cpp
        src/display/scenes/ProOpeningScene.cpp
        src/display/utils/getSharedFont.cpp
        src/display/utils/loadSound.cpp
        src/display/DisplayService.cpp
        src/display/scenes/DoubleStonesScene.cpp
)

set(MINMAX_FILES
        src/minmax/MinMax.cpp
        src/minmax/TranspositionTable.cpp
)

set(SERVICES_FILES
        src/services/AlignmentChecker.cpp
        src/services/CaptureService.cpp
        src/services/CheckLegalMove.cpp
        src/services/CheckWinService.cpp
        src/services/HeuristicService.cpp
)

set(SOURCE_FILES
        src/main.cpp
)

set(ALL_SOURCE_FILES
        ${BOARD_FILES}
        ${DISPLAY_FILES}
        ${MINMAX_FILES}
        ${SERVICES_FILES}
        ${SOURCE_FILES}
)

add_executable(${PROJECT_NAME} ${ALL_SOURCE_FILES})
target_compile_definitions(${PROJECT_NAME} PRIVATE)

target_include_directories(${PROJECT_NAME}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/board
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/display
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/display/components
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/display/scenes
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/display/utils
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/minmax
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/services
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/structs
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE SFML::Graphics
        PRIVATE SFML::Audio
)


#option(ENABLE_TESTS "Enable tests" ON)

#if (ENABLE_TESTS)
#    enable_testing()
#    add_subdirectory(tests)
#endif ()

