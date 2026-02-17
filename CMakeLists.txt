cmake_minimum_required(VERSION 3.29)
project(Gomoku)

include(CheckIPOSupported)
check_ipo_supported(RESULT result OUTPUT output)
if(result)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    message(STATUS "LTO/IPO enabled: This will take longer to link but run faster.")
else()
    message(WARNING "LTO/IPO not supported: ${output}")
endif()

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE MATCHES Debug)
    if(MSVC)
        # Options Visual Studio
        add_compile_options(
                /O2         # Maximize Speed
                /Oi         # Enable Intrinsic Functions (vital pour Bitboards)
                /Ot         # Favor Speed over Size
                /GL         # Whole Program Optimization
                /DNDEBUG    # Désactive les asserts
        )
        add_link_options(/LTCG) # Link Time Code Generation
    else()
        # Options GCC / Clang
        add_compile_options(
                -O3
                -march=native        # Utilise TOUTES les instructions de ton CPU (AVX2, BMI2...)
                -funroll-loops       # Déroule les boucles (rapide pour les parcours de tableaux)
                -fomit-frame-pointer # Libère un registre
                -DNDEBUG             # Désactive les asserts
        )
    endif()
endif()

option(USE_PGO_GEN "Generate Profile for PGO" OFF)
option(USE_PGO_USE "Use Profile for PGO" OFF)

if(USE_PGO_GEN)
    if(MSVC)
        add_link_options(/LTCG /GENPROFILE)
    else()
        add_compile_options(-fprofile-generate)
        add_link_options(-fprofile-generate)
    endif()
elseif(USE_PGO_USE)
    if(MSVC)
        add_link_options(/LTCG /USEPROFILE)
    else()
        add_compile_options(-fprofile-use -fprofile-correction)
        add_link_options(-fprofile-use)
    endif()
endif()

# ==========================================
# 4. DEPENDANCES
# ==========================================
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

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.4
)
FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# ==========================================
# 5. FICHIERS SOURCES
# ==========================================
set(BOARD_FILES
        src/board/Board.cpp
)

set(DISPLAY_FILES
        src/display/components/Button.cpp
        src/display/scenes/BoardScene.cpp
        src/display/scenes/MainMenuScene.cpp
        src/display/scenes/PvEScene.cpp
        src/display/scenes/PvPScene.cpp
        src/display/utils/getSharedFont.cpp
        src/display/DisplayService.cpp
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
        src/services/JsonService.cpp
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

# ==========================================
# 6. CIBLE ET LINKING
# ==========================================
add_executable(${PROJECT_NAME} ${ALL_SOURCE_FILES})

# UNITY BUILD : Fusionne les .cpp en un seul fichier géant avant compilation.
# Accélère la compilation ET permet au compilateur de mieux optimiser (inline agressif).
set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD ON)

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
        nlohmann_json::nlohmann_json
)

option(ENABLE_TESTS "Enable tests" ON)

if (ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()