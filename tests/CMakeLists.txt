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

# Configuration de CTest pour découvrir les tests Catch2
include(CTest)

# Ajoute le chemin du dossier `extras/` de Catch2 à la variable de modules
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

# Inclut Catch.cmake depuis Catch2 pour pouvoir utiliser `catch_discover_tests`
include(Catch)

# Enregistre les tests automatiquement avec CTest
catch_discover_tests(GomokuTests)

add_test(NAME RunAllTests COMMAND GomokuTests -s --reporter console)

