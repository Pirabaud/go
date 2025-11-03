#include "MoveOrdering.hpp"
#include "AlignmentChecker.hpp"
#include <algorithm>

MoveOrdering::MoveOrdering() {
    clearHistory();
    for (int d = 0; d < MAX_DEPTH; d++) {
        killerMoves[d][0] = {-1, -1};
        killerMoves[d][1] = {-1, -1};
    }
}

void MoveOrdering::clearHistory() {
    for (int x = 0; x < Board::SIZE; x++) {
        for (int y = 0; y < Board::SIZE; y++) {
            history[x][y] = 0;
        }
    }
}

void MoveOrdering::addKillerMove(int depth, Position move) {
    if (depth >= MAX_DEPTH) return;

    if (killerMoves[depth][0].x != move.x || killerMoves[depth][0].y != move.y) {
        killerMoves[depth][1] = killerMoves[depth][0];
        killerMoves[depth][0] = move;
    }
}

void MoveOrdering::updateHistory(Position move, int depth) {
    if (move.x >= 0 && move.x < Board::SIZE && move.y >= 0 && move.y < Board::SIZE) {
        history[move.x][move.y] += depth * depth;
    }
}

int MoveOrdering::getMoveScore(Position move, int depth, Position hashMove, const Board& board, bool isWhite) const {
    if (move.x == hashMove.x && move.y == hashMove.y) {
        return 10000000;
    }

    Board testBoard = board;
    auto undo = testBoard.makeMove(move, isWhite);
    int captureCount = undo.capturedStones.size() / 2;
    testBoard.undoMove(undo);

    if (captureCount > 0) {
        return 1000000 + captureCount * 100000;
    }

    // CRITIQUE: Détecter si ce coup bloque une menace adverse mortelle
    // Vérifier si l'adversaire aurait pu gagner/créer une menace à cette position
    Board oppTestBoard = board;
    if (isWhite) {
        oppTestBoard.addStoneBlack(move);
    } else {
        oppTestBoard.addStoneWhite(move);
    }

    const Board::StoneMask& oppGrid = isWhite ? oppTestBoard.getGridBlack() : oppTestBoard.getGridWhite();
    const Board::StoneMask& ourGrid = isWhite ? oppTestBoard.getGridWhite() : oppTestBoard.getGridBlack();

    // Bloquer un 5-en-ligne adverse imminent = PRIORITÉ MAXIMALE
    Alignment oppFive = AlignmentChecker::detectAlignment(move, 5, oppGrid, ourGrid);
    if (oppFive != Alignment::NOTALIGN) {
        return 5000000;  // URGENT: bloquer victoire adverse!
    }

    // Bloquer un 4-en-ligne libre adverse = TRÈS PRIORITAIRE
    Alignment oppFour = AlignmentChecker::detectAlignment(move, 4, oppGrid, ourGrid);
    if (oppFour == Alignment::FREE) {
        return 800000;  // Bloquer 4 libre adverse
    } else if (oppFour == Alignment::SEMIBLOCKED) {
        return 400000;  // Bloquer 4 semi-bloqué adverse
    }

    // Bloquer un 3-en-ligne libre adverse = PRIORITAIRE
    Alignment oppThree = AlignmentChecker::detectAlignment(move, 3, oppGrid, ourGrid);
    if (oppThree == Alignment::FREE) {
        return 200000;  // Bloquer 3 libre adverse
    }

    // OFFENSIF: Nos propres menaces (TRÈS IMPORTANT pour gagner!)
    Board offTestBoard = board;
    if (isWhite) {
        offTestBoard.addStoneWhite(move);
    } else {
        offTestBoard.addStoneBlack(move);
    }

    const Board::StoneMask& offOurGrid = isWhite ? offTestBoard.getGridWhite() : offTestBoard.getGridBlack();
    const Board::StoneMask& offTheirGrid = isWhite ? offTestBoard.getGridBlack() : offTestBoard.getGridWhite();

    // Créer un 5-en-ligne = VICTOIRE IMMÉDIATE
    Alignment ourFive = AlignmentChecker::detectAlignment(move, 5, offOurGrid, offTheirGrid);
    if (ourFive != Alignment::NOTALIGN) {
        return 10000000;  // GAGNER IMMÉDIATEMENT!
    }

    // Créer un 4-en-ligne libre = menace de victoire (très fort!)
    Alignment ourFour = AlignmentChecker::detectAlignment(move, 4, offOurGrid, offTheirGrid);
    if (ourFour == Alignment::FREE) {
        return 900000;  // Créer menace gagnante
    } else if (ourFour == Alignment::SEMIBLOCKED) {
        return 300000;  // Bon coup offensif
    }

    // Créer un 3-en-ligne libre = préparation d'attaque
    Alignment ourThree = AlignmentChecker::detectAlignment(move, 3, offOurGrid, offTheirGrid);
    if (ourThree == Alignment::FREE) {
        return 150000;  // Préparer menace
    }

    if (depth < MAX_DEPTH) {
        if (move.x == killerMoves[depth][0].x && move.y == killerMoves[depth][0].y) {
            return 10000;
        }
        if (move.x == killerMoves[depth][1].x && move.y == killerMoves[depth][1].y) {
            return 9000;
        }
    }

    int centerX = Board::SIZE / 2;
    int centerY = Board::SIZE / 2;
    int distToCenter = abs(move.x - centerX) + abs(move.y - centerY);
    int centerBonus = 100 - distToCenter * 2;

    return history[move.x][move.y] + centerBonus;
}

bool MoveOrdering::isThreatMove(Position move, const Board& board, bool isWhite) const {
    Board testBoard = board;
    if (isWhite) {
        testBoard.addStoneWhite(move);
    } else {
        testBoard.addStoneBlack(move);
    }

    const Board::StoneMask& grid = isWhite ? testBoard.getGridWhite() : testBoard.getGridBlack();
    const Board::StoneMask& oppGrid = isWhite ? testBoard.getGridBlack() : testBoard.getGridWhite();

    Alignment four = AlignmentChecker::detectAlignment(move, 4, grid, oppGrid);
    if (four == Alignment::FREE) {
        return true;
    }

    Alignment three = AlignmentChecker::detectAlignment(move, 3, grid, oppGrid);
    if (three == Alignment::FREE) {
        return true;
    }

    return false;
}

void MoveOrdering::orderMoves(std::vector<Position>& moves, int depth, Position hashMove, const Board& board, bool isWhite) {
    std::sort(moves.begin(), moves.end(), [&](const Position& a, const Position& b) {
        return getMoveScore(a, depth, hashMove, board, isWhite) > getMoveScore(b, depth, hashMove, board, isWhite);
    });

    // Réduction DRASTIQUE pour chercher beaucoup plus profond
    // Objectif: profondeur 8-10 pour voir les tactiques complexes
    int maxMoves = 6;   // Seulement les meilleurs à la racine
    if (depth > 3) {
        maxMoves = 5;   // Très sélectif en milieu de recherche
    }
    if (depth > 6) {
        maxMoves = 4;   // Ultra sélectif en profondeur
    }

    if (moves.size() > maxMoves) {
        // TOUJOURS inclure les coups critiques: captures ET défenses urgentes
        std::vector<Position> criticalMoves;
        std::vector<Position> normalMoves;

        for (const auto& move : moves) {
            bool isCritical = false;

            // Vérifier si c'est une capture
            Board testBoard = board;
            auto undo = testBoard.makeMove(move, isWhite);
            int captureCount = undo.capturedStones.size() / 2;
            testBoard.undoMove(undo);

            if (captureCount > 0) {
                isCritical = true;
            } else {
                // Vérifier défenses critiques ET menaces offensives
                Board checkBoard = board;
                if (isWhite) {
                    checkBoard.addStoneWhite(move);
                } else {
                    checkBoard.addStoneBlack(move);
                }

                const Board::StoneMask& ourGrid = isWhite ? checkBoard.getGridWhite() : checkBoard.getGridBlack();
                const Board::StoneMask& oppGrid = isWhite ? checkBoard.getGridBlack() : checkBoard.getGridWhite();

                // CRITIQUE si on crée un 5 ou un 4 (OFFENSIF)
                Alignment ourFive = AlignmentChecker::detectAlignment(move, 5, ourGrid, oppGrid);
                Alignment ourFour = AlignmentChecker::detectAlignment(move, 4, ourGrid, oppGrid);

                if (ourFive != Alignment::NOTALIGN || ourFour != Alignment::NOTALIGN) {
                    isCritical = true;  // Menace offensive critique!
                }

                // CRITIQUE si on bloque un 5 ou un 4 adverse (DÉFENSIF)
                Alignment oppFive = AlignmentChecker::detectAlignment(move, 5, oppGrid, ourGrid);
                Alignment oppFour = AlignmentChecker::detectAlignment(move, 4, oppGrid, ourGrid);

                if (oppFive != Alignment::NOTALIGN || oppFour != Alignment::NOTALIGN) {
                    isCritical = true;  // Défense critique!
                }
            }

            if (isCritical) {
                criticalMoves.push_back(move);
            } else {
                normalMoves.push_back(move);
            }
        }

        // Commencer avec TOUS les coups critiques (défense + captures)
        moves = criticalMoves;

        // Ajouter les coups normaux jusqu'à la limite
        int remaining = maxMoves - criticalMoves.size();
        for (int i = 0; i < remaining && i < normalMoves.size(); i++) {
            moves.push_back(normalMoves[i]);
        }
    }
}
