#pragma once

#include <cstdint>
#include <string_view>
#include <unordered_map>

/**
 * @brief Tile types define the terrain of each board cell.
 *
 * The board evolves over time: tiles can be created, destroyed, or transformed
 * by player abilities, enemy actions, or environmental events.
 */
enum class TileType : uint8_t {
    // ── Standard ──────────────────────────────────────────────────────────────
    Normal       = 0,   // Default walkable tile
    Wall         = 1,   // Impassable obstacle
    Water        = 2,   // Slows movement, blocks fire-based abilities
    Fire         = 3,   // Damages anything standing on it each turn
    Ice          = 4,   // Causes sliding — pieces overshoot their target
    Lava         = 5,   // Heavy damage, destroys non-resistant pieces
    Thorn        = 6,   // Damages on entry
    Healing      = 7,   // Heals the occupying piece per turn
    ManaWell     = 8,   // Restores ability charges
    Void         = 9,   // Pieces fall through and are removed from game
    Shield       = 10,  // Grants temporary defence buff
    Warp         = 11,  // Teleports pieces to a paired Warp tile
    Cursed       = 12,  // Applies a random debuff on entry
    Empowered    = 13,  // Boosts attack power
    Temporal     = 14,  // Grants an extra action
    COUNT        = 15
};

/// @brief Metadata about each tile type.
struct TileInfo {
    std::string_view name;
    bool             passable    = true;
    bool             destructible = false;
    int32_t          moveCost    = 1;   // additional movement cost (0 = free)
    int32_t          turnDamage  = 0;   // damage per turn while standing on it
};

/// @brief Lookup table for tile metadata.
inline constexpr TileInfo kTileInfo[] = {
    /* Normal    */ { "Normal",     true,  false, 1, 0 },
    /* Wall      */ { "Wall",       false, true,  0, 0 },
    /* Water     */ { "Water",      true,  false, 3, 0 },
    /* Fire      */ { "Fire",       true,  true,  1, 2 },
    /* Ice       */ { "Ice",        true,  true,  0, 0 },
    /* Lava      */ { "Lava",       false, true,  0, 5 },
    /* Thorn     */ { "Thorn",      true,  false, 1, 1 },
    /* Healing   */ { "Healing",    true,  false, 1, -3 },   // negative = heal
    /* ManaWell  */ { "ManaWell",   true,  false, 1, 0 },
    /* Void      */ { "Void",       false, false, 0, 0 },
    /* Shield    */ { "Shield",     true,  false, 1, 0 },
    /* Warp      */ { "Warp",       true,  true,  0, 0 },
    /* Cursed    */ { "Cursed",     true,  false, 2, 0 },
    /* Empowered */ { "Empowered",  true,  false, 1, 0 },
    /* Temporal  */ { "Temporal",   true,  false, 1, 0 },
};

/// @brief Returns the TileInfo for a given TileType.
[[nodiscard]] inline constexpr const TileInfo& getTileInfo(TileType type) {
    return kTileInfo[static_cast<uint8_t>(type)];
}
