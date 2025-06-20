# Set the minimum required version of cmake
cmake_minimum_required(VERSION 3.29)

# Store the name of the project in PROJECT_NAME
project(Gomoku)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(LIBS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs)


# Contain all cpp files within src
set(SOURCE_FILES
        src/main.cpp
        src/board/Board.cpp
        include/board/Board.h
)

# Contain all source files
set(ALL_SOURCE_FILES
        ${SOURCE_FILES}
)

# Add an executable target
add_executable(${PROJECT_NAME} ${ALL_SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/board
)

target_link_libraries(${PROJECT_NAME})