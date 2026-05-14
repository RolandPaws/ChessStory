#pragma once

#include "core/Tile.h"
#include "utils/Vector2D.h"

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <memory>

// ── Forward declarations ─────────────────────────────────────────────────────
class Piece;

/**
 * @brief The dynamic chess board.
 *
 * The board is a grid of Tiles that can change over time.  The board size
 * is larger than a standard 8×8 chess board (e.g. 12×12 or 16×16) and can
 * grow or shrink during gameplay.
 */
class Board {
public:
    static constexpr int32_t kDefaultSize = 12;

    explicit Board(int32_t size = kDefaultSize);

    // ── Grid access ───────────────────────────────────────────────────────────
    [[nodiscard]] int32_t width()  const noexcept { return width_; }
    [[nodiscard]] int32_t height() const noexcept { return height_; }

    [[nodiscard]] Tile&       at(Vector2D pos);
    [[nodiscard]] const Tile& at(Vector2D pos) const;

    [[nodiscard]] bool inBounds(Vector2D pos) const noexcept;

    // ── Piece management ──────────────────────────────────────────────────────
    [[nodiscard]] std::shared_ptr<Piece> pieceAt(Vector2D pos) const;
    void placePiece(std::shared_ptr<Piece> piece, Vector2D pos);
    [[nodiscard]] std::shared_ptr<Piece> removePiece(Vector2D pos);
    void movePiece(Vector2D from, Vector2D to);

    [[nodiscard]] std::vector<std::shared_ptr<Piece>> allPieces() const;

    // ── Tile mutation ─────────────────────────────────────────────────────────
    void setTileType(Vector2D pos, TileType type);
    void addTemporaryTile(Vector2D pos, TileType type, int32_t duration);

    /// Evolve the board each turn (tile durations, random events, etc.)
    void evolve();

    // ── Utility ───────────────────────────────────────────────────────────────
    void reset();

private:
    int32_t width_  = kDefaultSize;
    int32_t height_ = kDefaultSize;

    // 2D grid stored as flat vector for cache locality
    std::vector<Tile> grid_;

    // Piece lookup: position → piece pointer
    std::unordered_map<Vector2D, std::shared_ptr<Piece>> pieces_;

    // ── Helpers ───────────────────────────────────────────────────────────────
    [[nodiscard]] int32_t index(Vector2D pos) const noexcept;
    void generateStandardLayout();
};
