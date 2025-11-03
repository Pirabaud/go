#include <iostream>
#include <chrono>
#include "Board.h"
#include "MinMax.hpp"

void benchmarkDepth(Board& board, int depth, const std::string& scenario) {
    std::cout << "\n========================================" << std::endl;
    std::cout << "Scenario: " << scenario << std::endl;
    std::cout << "========================================" << std::endl;

    MinMax::MAX_DEPTH = depth;
    MinMax ai(board);

    auto start = std::chrono::high_resolution_clock::now();
    auto [move, elapsedMs] = ai.run(500);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "\nBest move: (" << move.x << ", " << move.y << ")" << std::endl;
    std::cout << "Time: " << elapsedMs << "ms" << std::endl;
    std::cout << "Status: " << (elapsedMs < 500 ? "✓ PASSED" : "✗ FAILED") << " (target: < 500ms)" << std::endl;
}

int main() {
    std::cout << "===========================================" << std::endl;
    std::cout << "      GOMOKU AI PERFORMANCE BENCHMARK     " << std::endl;
    std::cout << "===========================================" << std::endl;

    Board emptyBoard;
    benchmarkDepth(emptyBoard, 10, "Empty board (depth 10)");

    Board earlyGame;
    earlyGame.addStoneBlack({9, 9});
    earlyGame.addStoneWhite({9, 10});
    earlyGame.addStoneBlack({10, 10});
    earlyGame.addStoneWhite({8, 9});
    benchmarkDepth(earlyGame, 10, "Early game - 4 stones (depth 10)");

    Board midGame;
    midGame.addStoneBlack({9, 9});
    midGame.addStoneWhite({9, 10});
    midGame.addStoneBlack({10, 10});
    midGame.addStoneWhite({8, 9});
    midGame.addStoneBlack({8, 8});
    midGame.addStoneWhite({10, 11});
    midGame.addStoneBlack({11, 11});
    midGame.addStoneWhite({7, 8});
    midGame.addStoneBlack({7, 7});
    midGame.addStoneWhite({11, 12});
    benchmarkDepth(midGame, 10, "Mid game - 10 stones (depth 10)");

    Board tacticalPosition;
    tacticalPosition.addStoneBlack({9, 9});
    tacticalPosition.addStoneBlack({9, 10});
    tacticalPosition.addStoneBlack({9, 11});
    tacticalPosition.addStoneWhite({8, 9});
    tacticalPosition.addStoneWhite({10, 9});
    benchmarkDepth(tacticalPosition, 10, "Tactical position - Threat detection (depth 10)");

    std::cout << "\n===========================================" << std::endl;
    std::cout << "         BENCHMARK COMPLETE               " << std::endl;
    std::cout << "===========================================" << std::endl;

    return 0;
}
