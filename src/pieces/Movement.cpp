#include "pieces/Movement.h"
#include "core/Board.h"
#include "pieces/Piece.h"

#include <algorithm>
#include <set>

// ── Static builders ───────────────────────────────────────────────────────────

MovementPattern MovementPattern::Sliding(std::vector<Vector2D> dirs, int32_t maxRange) {
    return {std::move(dirs), maxRange, false, true, nullptr};
}

MovementPattern MovementPattern::Leap(std::vector<Vector2D> offsets) {
    return {std::move(offsets), 1, true, true, nullptr};
}

MovementPattern MovementPattern::Step(std::vector<Vector2D> steps) {
    return {std::move(steps), 1, false, true, nullptr};
}

MovementPattern MovementPattern::Custom(Validator validator) {
    return {{}, 0, false, true, std::move(validator)};
}

// ── Pattern definitions ───────────────────────────────────────────────────────

std::vector<MovementPattern> MovementSystem::getPatterns(PieceType type) {
    using Pat = MovementPattern;

    switch (type) {
    // ── Classical ─────────────────────────────────────────────────────────────
    case PieceType::King:
        return { Pat::Step({
            { 1, 0}, {-1, 0}, { 0, 1}, { 0,-1},
            { 1, 1}, { 1,-1}, {-1, 1}, {-1,-1}
        })};

    case PieceType::Queen:
        return { Pat::Sliding({
            { 1, 0}, {-1, 0}, { 0, 1}, { 0,-1},
            { 1, 1}, { 1,-1}, {-1, 1}, {-1,-1}
        })};

    case PieceType::Rook:
        return { Pat::Sliding({{ 1, 0}, {-1, 0}, { 0, 1}, { 0,-1}}) };

    case PieceType::Bishop:
        return { Pat::Sliding({{ 1, 1}, { 1,-1}, {-1, 1}, {-1,-1}}) };

    case PieceType::Knight:
        return { Pat::Leap({
            { 2, 1}, { 2,-1}, {-2, 1}, {-2,-1},
            { 1, 2}, { 1,-2}, {-1, 2}, {-1,-2}
        })};

    case PieceType::Pawn:
        return { Pat::Step({{ 0, 1}}) };   // Forward 1

    // ── Extended ranged / caster ──────────────────────────────────────────────
    case PieceType::Archer: {
        auto move = Pat::Step({{ 1, 0}, {-1, 0}, { 0, 1}, { 0,-1}});
        auto ranged = Pat::Sliding({{ 1, 0}, {-1, 0}, { 0, 1}, { 0,-1}}, 3);
        ranged.customValidator = [](const Board&, const Piece&, Vector2D, Vector2D) {
            return true;
        };
        return {move, ranged};
    }

    // ── Extended melee / hybrid ───────────────────────────────────────────────
    case PieceType::Mage:
        return { Pat::Step({{ 1, 0}, {-1, 0}, { 0, 1}, { 0,-1}}),
                 Pat::Custom([](const Board&, const Piece&, Vector2D, Vector2D) { return true; }) };

    case PieceType::Paladin:
        return { Pat::Step({
            { 1, 0}, {-1, 0}, { 0, 1}, { 0,-1},
            { 1, 1}, { 1,-1}, {-1, 1}, {-1,-1}
        })};

    case PieceType::Assassin:
        return { Pat::Step({{ 1, 0}, {-1, 0}, { 0, 1}, { 0,-1}}) };

    case PieceType::Warlock:
        return { Pat::Step({{ 1, 0}, {-1, 0}, { 0, 1}, { 0,-1}}) };

    case PieceType::Druid:
        return { Pat::Step({{ 1, 0}, {-1, 0}, { 0, 1}, { 0,-1}}) };

    case PieceType::Berserker:
        return { Pat::Step({
            { 1, 0}, {-1, 0}, { 0, 1}, { 0,-1},
            { 1, 1}, { 1,-1}, {-1, 1}, {-1,-1}
        })};

    case PieceType::Necromancer:
        return { Pat::Step({{ 1, 0}, {-1, 0}, { 0, 1}, { 0,-1}}) };

    case PieceType::Valkyrie:
        // Fly — can land on any tile within range, ignoring obstacles
        return { Pat::Sliding({
            { 1, 1}, { 1,-1}, {-1, 1}, {-1,-1},
            { 1, 0}, {-1, 0}, { 0, 1}, { 0,-1}
        }, 3) };

    case PieceType::Engineer:
        return { Pat::Step({{ 1, 0}, {-1, 0}, { 0, 1}, { 0,-1}}) };

    case PieceType::Golem:
        return { Pat::Step({{ 1, 0}, {-1, 0}, { 0, 1}, { 0,-1}}) };

    case PieceType::Phantom: {
        // Same as king but ignores wall passability (handled in validation)
        return { Pat::Step({
            { 1, 0}, {-1, 0}, { 0, 1}, { 0,-1},
            { 1, 1}, { 1,-1}, {-1, 1}, {-1,-1}
        })};
    }

    case PieceType::Mimic:
        return { Pat::Step({
            { 1, 0}, {-1, 0}, { 0, 1}, { 0,-1},
            { 1, 1}, { 1,-1}, {-1, 1}, {-1,-1}
        })};

    case PieceType::Chronomancer:
        return { Pat::Step({
            { 1, 0}, {-1, 0}, { 0, 1}, { 0,-1},
            { 1, 1}, { 1,-1}, {-1, 1}, {-1,-1}
        })};

    default:
        return {};
    }
}

// ── Move computation ─────────────────────────────────────────────────────────

std::vector<Vector2D> MovementSystem::getValidMoves(const Board& board,
                                                     const Piece& piece,
                                                     Vector2D from) {
    const auto patterns = getPatterns(piece.type());
    std::set<Vector2D> validSet;  // using set to deduplicate

    for (const auto& pattern : patterns) {
        if (pattern.customValidator && pattern.directions.empty()) {
            // Fully custom pattern — invoke validator directly on all board tiles
            for (int32_t y = 0; y < board.height(); ++y) {
                for (int32_t x = 0; x < board.width(); ++x) {
                    Vector2D to{x, y};
                    if (to != from && pattern.customValidator(board, piece, from, to)) {
                        validSet.insert(to);
                    }
                }
            }
            continue;
        }

        for (const auto& dir : pattern.directions) {
            if (pattern.jumps) {
                // Leaping — check each offset directly
                Vector2D to = from + dir;
                if (board.inBounds(to)) {
                    auto targetPiece = board.pieceAt(to);
                    bool canCapture = targetPiece && targetPiece->faction() != piece.faction();
                    bool empty = !targetPiece;
                    if (empty || canCapture) {
                        validSet.insert(to);
                    }
                }
            } else {
                // Sliding / stepping — iterate along direction
                int32_t steps = 0;
                Vector2D to = from + dir;
                while (board.inBounds(to) && (pattern.maxSteps == 0 || steps < pattern.maxSteps)) {
                    auto targetPiece = board.pieceAt(to);

                    if (!board.at(to).passable() && !piece.canPhase()) {
                        if (!piece.canPhase()) break;
                    }

                    if (targetPiece) {
                        if (targetPiece->faction() != piece.faction()) {
                            validSet.insert(to);  // capture
                        }
                        break;  // blocked
                    }

                    validSet.insert(to);

                    ++steps;
                    to += dir;
                }
            }
        }
    }

    return {validSet.begin(), validSet.end()};
}
