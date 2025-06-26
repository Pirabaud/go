cmake_minimum_required(VERSION 3.29)
project(Gomoku)

# Activer les tests dès le début
enable_testing()

# Utiliser C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Flags pour profilage
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")

# Include général
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# SFML (affichage)
include(FetchContent)
set(SFML_BUILD_AUDIO OFF)
set(SFML_BUILD_NETWORK OFF)
set(SFML_BUILD_WINDOW ON)
set(SFML_BUILD_GRAPHICS ON)
set(SFML_BUILD_SYSTEM ON)

FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.1
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

# Catch2 pour tests
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.4
)
FetchContent_MakeAvailable(Catch2)

# Ajoute le chemin du module Catch2 pour catch_discover_tests
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch) # pour catch_discover_tests

# Sources de ton application principale
set(BOARD_FILES
        src/board/Board.cpp
        src/board/CheckWinService.cpp
)

set(DISPLAY_COMPONENTS_FILES
        src/display/components/Button.cpp
        src/display/utils/getSharedFont.cpp
)

set(DISPLAY_SCENES_FILES
        src/display/scenes/BoardScene.cpp
        src/display/scenes/MainMenuScene.cpp
        src/display/scenes/PvPScene.cpp
)

set(DISPLAY_FILES
        ${DISPLAY_COMPONENTS_FILES}
        ${DISPLAY_SCENES_FILES}
        src/display/DisplayService.cpp
)

set(SOURCE_FILES
        src/main.cpp
        src/board/Board.cpp
        include/board/Board.h
)

set(ALL_SOURCE_FILES
        ${BOARD_FILES}
        ${DISPLAY_FILES}
        ${SOURCE_FILES}
)

add_executable(${PROJECT_NAME} ${ALL_SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/board
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/display
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/display/components
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/display/scenes
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/services
)

target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics)


option(ENABLE_TESTS "Enable tests" ON)

if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

