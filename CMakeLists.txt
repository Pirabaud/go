# Set the minimum required version of cmake
cmake_minimum_required(VERSION 3.29)

# Store the name of the project in PROJECT_NAME
project(Gomoku)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(LIBS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs)

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

# Contain all cpp files within src
set(SOURCE_FILES
        src/main.cpp
        src/board/Board.cpp
        include/board/Board.h
)

# Contain all source files
set(ALL_SOURCE_FILES
        ${BOARD_FILES}
        ${DISPLAY_FILES}
        ${SOURCE_FILES}
)


# Add an executable target
add_executable(${PROJECT_NAME} ${ALL_SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/board
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/display
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/display/components
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/display/scenes
)

target_link_libraries(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics)