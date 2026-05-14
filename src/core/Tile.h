#pragma once

#include "TileType.h"
#include "utils/Vector2D.h"
#include <cstdint>
#include <optional>

/**
 * @brief A single cell on the chess board.
 *
 * Tiles are dynamic: their type can change over time due to abilities,
 * items, or board-evolution events.
 */
class Tile {
public:
    Tile() noexcept = default;
    explicit Tile(TileType type, Vector2D position = {}) noexcept;

    // ── Accessors ─────────────────────────────────────────────────────────────
    [[nodiscard]] TileType       type()     const noexcept { return type_; }
    [[nodiscard]] Vector2D       position() const noexcept { return position_; }
    [[nodiscard]] const TileInfo& info()    const noexcept { return getTileInfo(type_); }
    [[nodiscard]] bool           passable() const noexcept { return info().passable; }
    [[nodiscard]] int32_t        moveCost() const noexcept { return info().moveCost; }

    // ── Mutators ──────────────────────────────────────────────────────────────
    void setType(TileType type) noexcept { type_ = type; }
    void setPosition(Vector2D pos) noexcept { position_ = pos; }

    // ── State ─────────────────────────────────────────────────────────────────
    [[nodiscard]] bool hasWarpTarget()      const noexcept { return warpTarget_.has_value(); }
    [[nodiscard]] std::optional<Vector2D> warpTarget() const noexcept { return warpTarget_; }
    void setWarpTarget(Vector2D target) noexcept { warpTarget_ = target; }
    void clearWarpTarget() noexcept { warpTarget_.reset(); }

    [[nodiscard]] int32_t remainingTurns() const noexcept { return remainingTurns_; }
    void setRemainingTurns(int32_t t) noexcept { remainingTurns_ = t; }
    void decrementTurns() noexcept { if (remainingTurns_ > 0) --remainingTurns_; }

private:
    TileType  type_          = TileType::Normal;
    Vector2D  position_;
    int32_t   remainingTurns_ = -1;   // -1 = permanent

    // Warp pair target (only meaningful for Warp tiles)
    std::optional<Vector2D> warpTarget_;
};
