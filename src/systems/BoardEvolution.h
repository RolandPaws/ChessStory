#pragma once

#include <cstdint>
#include <memory>

class Board;
class TurnSystem;

/**
 * @brief Drives the dynamic board changes each turn.
 *
 * The chess board is not static — tiles can appear, disappear, transform,
 * or spread over time.  This system manages those environmental changes
 * to keep the battlefield fresh and challenging.
 *
 * Events include:
 * - Tile type spread (fire spreads, ice melts, etc.)
 * - Random tile generation
 * - Earthquake (shifts rows/columns)
 * - Tile decay (temporary tiles expire)
 * - Environmental hazards (meteor showers, plant growth)
 */
class BoardEvolution {
public:
    explicit BoardEvolution(std::shared_ptr<Board> board);

    /// Evolve the board for one turn.
    void evolve(int32_t turnNumber);

    /// Set the evolution frequency (every N turns).
    void setFrequency(int32_t turns) noexcept { frequency_ = turns; }

    /// Manually trigger a specific event.
    void triggerSpread(TileType type);
    void triggerEarthquake();
    void triggerRandomGrowth();

private:
    std::shared_ptr<Board> board_;
    int32_t frequency_ = 3;  // Every 3 turns

    // ── Event helpers ─────────────────────────────────────────────────────
    void spreadTileType(TileType type, int32_t radius = 1);
    void meltIceTiles();
    void growThorns();
    void randomTileMutation();
};
