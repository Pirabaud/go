# tests/CMakeLists.txt

add_executable(GomokuTests
        services/test_checkMoveService.cpp
        ../src/services/CheckMoveService.cpp
        ../src/board/Board.cpp
)

target_link_libraries(GomokuTests
        PRIVATE Catch2::Catch2WithMain
)

target_include_directories(GomokuTests
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include/board
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

# Enregistrement des tests avec CTest via Catch2 v3
catch_discover_tests(GomokuTests)

