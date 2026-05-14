#pragma once

#include "utils/Vector2D.h"
#include "pieces/PieceType.h"

#include <cstdint>
#include <vector>
#include <functional>

class Board;
class Piece;

/**
 * @brief Movement pattern describes how a piece can traverse the board.
 *
 * Supports classical chess movements plus arbitrary custom patterns for
 * the extended roguelike pieces.
 */
struct MovementPattern {
    using Validator = std::function<bool(const Board&, const Piece&, Vector2D from, Vector2D to)>;

    /// Direction vectors that define movement axes (e.g. {1,0} for right).
    std::vector<Vector2D> directions;

    /// Maximum number of steps in each direction (0 = unlimited / full range).
    int32_t maxSteps = 0;

    /// If true, the piece can jump over pieces (like a knight).
    bool jumps = false;

    /// If true, the piece can capture on landing but not on intermediate squares.
    bool captureOnlyOnLanding = true;

    /// Optional custom validator for complex movement rules.
    Validator customValidator = nullptr;

    /// Build a standard sliding movement (like rook, bishop, queen).
    static MovementPattern Sliding(std::vector<Vector2D> dirs, int32_t maxRange = 0);

    /// Build a leaping movement (like knight).
    static MovementPattern Leap(std::vector<Vector2D> offsets);

    /// Build a stepped movement (like pawn — 1 step forward, etc.).
    static MovementPattern Step(std::vector<Vector2D> steps);

    /// Custom pattern from a lambda.
    static MovementPattern Custom(Validator validator);
};

/**
 * @brief Central movement system that computes valid moves for any piece.
 */
class MovementSystem {
public:
    /// Returns the movement pattern(s) for a given piece type.
    [[nodiscard]] static std::vector<MovementPattern> getPatterns(PieceType type);

    /// Compute all valid destinations for a piece at `from` on the given board.
    [[nodiscard]] static std::vector<Vector2D> getValidMoves(const Board& board,
                                                              const Piece& piece,
                                                              Vector2D from);
};
