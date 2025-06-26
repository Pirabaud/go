# tests/CMakeLists.txt
cmake_minimum_required(VERSION 3.29)

add_executable(GomokuTests
        services/test_checkMoveService.cpp
        services/CheckWinServiceTest.cpp
        ../src/services/CheckMoveService.cpp
        ../src/services/CheckWinService.cpp
        ../src/board/Board.cpp
)

target_link_libraries(GomokuTests
        PRIVATE Catch2::Catch2WithMain
)


target_include_directories(GomokuTests
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include/board
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include/structs
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include/services
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../cmake-build-debug/_deps/sfml-src/include
)

catch_discover_tests(GomokuTests)

add_test(NAME RunAllTests COMMAND GomokuTests -s --reporter console)

