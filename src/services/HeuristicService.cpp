#include "HeuristicService.h"
#include <cmath>
#include <iostream>

#include "AlignmentChecker.hpp"
#include "CheckWinService.hpp"
#include "FreeThreeService.h"
#include "CaptureService.hpp"

#define WIN_WEIGHT 1000000
#define CAPTURE_WEIGHT 8000
#define CAPTURE_THREAT_WEIGHT 2000
#define IMMEDIATE_CAPTURE_WEIGHT 15000  // Capture possible au prochain coup
#define FOUR_OPEN_WEIGHT 5000
#define THREE_OPEN_WEIGHT 1000
#define TWO_OPEN_WEIGHT 100

int HeuristicService::getHeuristicValue(Board& board) {
    // Vérifier victoire immédiate
    const sf::Color* winner = CheckWinService::isWin(board);
    if (winner) {
        return (*winner == sf::Color::White) ? WIN_WEIGHT : -WIN_WEIGHT;
    }

    int score = 0;

    int whiteCaptured = board.getWhiteCaptured();
    int blackCaptured = board.getBlackCaptured();

    int captureBalance = blackCaptured - whiteCaptured;
    score += captureBalance * CAPTURE_WEIGHT;

    // 2. MENACES DE CAPTURE IMMÉDIATES
    score += evaluateImmediateCaptureThreats(board);

    // 3. ÉVALUER LES ALIGNEMENTS
    for (int x = 0; x < Board::SIZE; ++x) {
        for (int y = 0; y < Board::SIZE; ++y) {
            Position pos{x, y};

            if (board.isWhiteStoneAt(pos)) {
                score += 1;
                score += evaluateStonePatterns(board, pos, true);
            }
            else if (board.isBlackStoneAt(pos)) {
                score -= 1;
                score -= evaluateStonePatterns(board, pos, false);
            }
        }
    }

    return score;
}

int HeuristicService::evaluateStonePatterns(Board& board, Position pos, bool isWhite) {
    int stoneValue = 0;

    // Patterns gagnants (5 alignés)
    if (AlignmentChecker::detectAlignment(pos, 5,
        isWhite ? board.getGridWhite() : board.getGridBlack(),
        isWhite ? board.getGridBlack() : board.getGridWhite()) != Alignment::NOTALIGN) {
        return WIN_WEIGHT / 10;
    }

    // Quatre alignés ouvert
    Alignment fourAlign = AlignmentChecker::detectAlignment(pos, 4,
        isWhite ? board.getGridWhite() : board.getGridBlack(),
        isWhite ? board.getGridBlack() : board.getGridWhite());
    if (fourAlign == Alignment::FREE) {
        stoneValue += FOUR_OPEN_WEIGHT;
    } else if (fourAlign == Alignment::SEMIBLOCKED) {
        stoneValue += FOUR_OPEN_WEIGHT / 2;
    }

    // Trois alignés ouvert
    Alignment threeAlign = AlignmentChecker::detectAlignment(pos, 3,
        isWhite ? board.getGridWhite() : board.getGridBlack(),
        isWhite ? board.getGridBlack() : board.getGridWhite());
    if (threeAlign == Alignment::FREE) {
        stoneValue += THREE_OPEN_WEIGHT;
    } else if (threeAlign == Alignment::SEMIBLOCKED) {
        stoneValue += THREE_OPEN_WEIGHT / 2;
    }

    // Deux alignés ouvert
    Alignment twoAlign = AlignmentChecker::detectAlignment(pos, 2,
        isWhite ? board.getGridWhite() : board.getGridBlack(),
        isWhite ? board.getGridBlack() : board.getGridWhite());
    if (twoAlign == Alignment::FREE) {
        stoneValue += TWO_OPEN_WEIGHT;
    }

    return stoneValue;
}

int HeuristicService::evaluateImmediateCaptureThreats(Board& board) {
    int threatScore = 0;

    // Évaluer les captures possibles pour les blancs
    threatScore += evaluatePlayerCaptureThreats(board, true);

    // Évaluer les captures possibles pour les noirs (soustraire)
    threatScore -= evaluatePlayerCaptureThreats(board, false);

    return threatScore;
}

int HeuristicService::evaluatePlayerCaptureThreats(Board& board, bool isWhite) {
    int threatScore = 0;

    // Pour chaque case vide du plateau
    for (int x = 0; x < Board::SIZE; ++x) {
        for (int y = 0; y < Board::SIZE; ++y) {
            Position pos{x, y};

            // Vérifier si cette case est libre
            if (!board.isWhiteStoneAt(pos) && !board.isBlackStoneAt(pos)) {

                // Tester si jouer ici créerait une capture
                Board testBoard = board; // Copie pour test

                if (isWhite) {
                    testBoard.addStoneWhite(pos);
                } else {
                    testBoard.addStoneBlack(pos);
                }

                // Vérifier si ce coup crée une capture
                if (CaptureService::resolveCaptureAtPosition(testBoard, pos, isWhite)) {
                    threatScore += isWhite ? IMMEDIATE_CAPTURE_WEIGHT : -IMMEDIATE_CAPTURE_WEIGHT;
                    board.resolveCaptures();
                }

                // Vérifier aussi les menaces de capture (configurations à 1 coup d'une capture)
                threatScore += evaluateCapturePatterns(board, pos, isWhite);
            }
        }
    }

    return threatScore;
}

int HeuristicService::evaluateCapturePatterns(Board& board, Position emptyPos, bool isWhite) {
    int patternScore = 0;

    const Board::StoneMask& allyMask = isWhite ? board.getGridWhite() : board.getGridBlack();
    const Board::StoneMask& enemyMask = isWhite ? board.getGridBlack() : board.getGridWhite();

    std::array directions = {
        std::make_pair(0, 1),
        std::make_pair(1, 0),
        std::make_pair(1, -1),
        std::make_pair(1, 1),
    };

    // Vérifier les patterns qui mènent à des captures
    for (auto& [dx, dy] : directions) {
        // Pattern 1: _XOO_ (où X=adversaire, O=allié, _=vide)
        // Si on joue sur le vide, on peut capturer
        if (isCapturePattern(board, emptyPos, {dx, dy}, allyMask, enemyMask)) {
            patternScore += CAPTURE_THREAT_WEIGHT;
        }
    }

    return patternScore;
}

bool HeuristicService::isCapturePattern(Board& board, Position pos, Position dir,
                                       const Board::StoneMask& allyMask,
                                       const Board::StoneMask& enemyMask) {
    // Vérifier les positions pour le pattern de capture
    // Pattern: [vide] - [adversaire] - [allié] - [allié] - [vide]

    Position positions[] = {
        {pos.x + dir.x, pos.y + dir.y},
        {pos.x + dir.x * 2, pos.y + dir.y * 2},
        {pos.x + dir.x * 3, pos.y + dir.y * 3},
        {pos.x - dir.x, pos.y - dir.y},
        {pos.x - dir.x * 2, pos.y - dir.y * 2}
    };

    // Vérifier les bornes
    for (const auto& p : positions) {
        if (p.x < 0 || p.x >= Board::SIZE || p.y < 0 || p.y >= Board::SIZE) {
            return false;
        }
    }

    // Pattern dans une direction: vide - ennemi - allié - allié - vide
    bool pattern1 =
        !Board::isStoneAt(allyMask, positions[0]) && !Board::isStoneAt(enemyMask, positions[0]) && // vide
        Board::isStoneAt(enemyMask, positions[1]) && // ennemi
        Board::isStoneAt(allyMask, positions[2]) && // allié
        Board::isStoneAt(allyMask, positions[3]) && // allié
        !Board::isStoneAt(allyMask, positions[4]) && !Board::isStoneAt(enemyMask, positions[4]); // vide

    // Pattern dans l'autre direction: même logique
    bool pattern2 =
        !Board::isStoneAt(allyMask, positions[4]) && !Board::isStoneAt(enemyMask, positions[4]) && // vide
        Board::isStoneAt(enemyMask, positions[3]) && // ennemi
        Board::isStoneAt(allyMask, positions[2]) && // allié
        Board::isStoneAt(allyMask, positions[1]) && // allié
        !Board::isStoneAt(allyMask, positions[0]) && !Board::isStoneAt(enemyMask, positions[0]); // vide

    return pattern1 || pattern2;
}