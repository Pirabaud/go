cmake_minimum_required(VERSION 3.29)

add_executable(GomokuTests
        board/BoardTest.cpp
        minmax/MinMaxTest.cpp
        services/CheckAlignmentCheckerTest.cpp
        services/CheckLegalMoveTest.cpp
        services/CheckWinServiceTest.cpp
        ../src/services/AlignmentChecker.cpp
        ../src/services/CheckLegalMove.cpp
        ../src/services/CheckWinService.cpp
        ../src/services/CaptureService.cpp
        ../src/board/Board.cpp
        ../src/minmax/MinMax.cpp
        ../src/services/HeuristicService.cpp
        ../src/services/JsonService.cpp
        ../src/minmax/TranspositionTable.cpp
)

target_link_libraries(GomokuTests PRIVATE SFML::Graphics nlohmann_json::nlohmann_json)

target_link_libraries(GomokuTests
        PRIVATE Catch2::Catch2WithMain
)


target_include_directories(GomokuTests
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include/board
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include/minmax
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include/structs
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include/services
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../cmake-build-debug/_deps/sfml-src/include
)

catch_discover_tests(GomokuTests)

add_test(NAME RunAllTests COMMAND GomokuTests -s --reporter console)