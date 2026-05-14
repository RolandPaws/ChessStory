#pragma once

#include <cstdint>
#include <string_view>

/**
 * @brief Faction alignment for pieces.
 */
enum class Faction : uint8_t {
    Player   = 0,
    Enemy    = 1,
    Neutral  = 2,
};

/**
 * @brief All piece types in the game.
 *
 * This goes far beyond classical chess. The taxonomy separates standard
 * chess pieces from extended "roguelike" pieces that have unique abilities.
 */
enum class PieceType : uint8_t {
    // ── Classical pieces ──────────────────────────────────────────────────────
    King     = 0,
    Queen    = 1,
    Rook     = 2,
    Bishop   = 3,
    Knight   = 4,
    Pawn     = 5,

    // ── Extended pieces ───────────────────────────────────────────────────────
    Archer   = 6,   // Ranged attack (2–3 tiles), cannot melee
    Mage     = 7,   // Area-of-effect attack, can convert tiles
    Paladin  = 8,   // Tanky, heals adjacent allies
    Assassin = 9,   // Can teleport behind enemies, high crit
    Warlock  = 10,  // Curses enemies, drains health
    Druid    = 11,  // Can change tile types
    Berserker= 12,  // Gains power as HP decreases
    Necromancer=13, // Summons temporary skeleton pieces from fallen enemies
    Valkyrie = 14,  // Can fly over obstacles, resurrects once
    Engineer = 15,  // Places temporary walls / traps
    Golem    = 16,  // Slow but extremely durable, push immune
    Phantom  = 17,  // Can phase through walls
    Mimic    = 18,  // Copies the movement of the last enemy hit
    Chronomancer=19,// Can rewind own position, grant extra turns
    COUNT    = 20,
};

/// @brief Metadata for each piece type.
struct PieceInfo {
    std::string_view name;
    std::string_view description;
    int32_t          baseHP;
    int32_t          baseAttack;
    int32_t          baseDefence;
    int32_t          baseMoveRange;
    bool             canFly    = false;
    bool             canPhase  = false;   // pass through walls
};

/// @brief Lookup table for piece metadata.
inline constexpr PieceInfo kPieceInfo[] = {
    /* King        */ { "King",        "The royal leader – must be protected",           10, 2, 1, 1 },
    /* Queen       */ { "Queen",       "The most versatile classical piece",              8, 4, 2, 8 },
    /* Rook        */ { "Rook",        "Straight-line powerhouse",                        7, 3, 3, 7 },
    /* Bishop      */ { "Bishop",      "Diagonal specialist",                             6, 3, 2, 7 },
    /* Knight      */ { "Knight",      "Leaping flanker",                                 6, 3, 1, 2, false, false },
    /* Pawn        */ { "Pawn",        "The humble frontline – can promote",               3, 1, 0, 1 },
    /* Archer      */ { "Archer",      "Deadly at range, vulnerable up close",             5, 3, 1, 1, false, false },
    /* Mage        */ { "Mage",        "Wields area magic and tile manipulation",           6, 4, 0, 2, false, false },
    /* Paladin     */ { "Paladin",     "Holy defender with healing aura",                 12, 3, 4, 2, false, false },
    /* Assassin    */ { "Assassin",    "Shadow-teleporting backstabber",                   4, 5, 0, 3, false, false },
    /* Warlock     */ { "Warlock",     "Curses foes and steals life essence",              7, 3, 1, 2, false, false },
    /* Druid       */ { "Druid",       "Commands nature – changes the battlefield",        7, 2, 2, 2, false, false },
    /* Berserker   */ { "Berserker",   "Fury grows as wounds accumulate",                 10, 2, 1, 2, false, false },
    /* Necromancer */ { "Necromancer", "Raises the dead to fight for them",                6, 2, 1, 2, false, false },
    /* Valkyrie    */ { "Valkyrie",    "Wings and valour – resurrects once upon death",    8, 3, 2, 3, true,  false },
    /* Engineer    */ { "Engineer",    "Deploys constructs and traps on the battlefield",   7, 2, 2, 2, false, false },
    /* Golem       */ { "Golem",       "Immovable object, unstoppable force",             15, 2, 5, 1, false, false },
    /* Phantom     */ { "Phantom",     "Glides through walls and obstacles",               5, 3, 1, 3, false, true  },
    /* Mimic       */ { "Mimic",       "Adapts – copies the last enemy movement pattern",  6, 3, 2, 2, false, false },
    /* Chronomancer*/ { "Chronomancer","Twists time – rewinds and grants extra turns",     5, 2, 1, 2, false, false },
};

[[nodiscard]] inline constexpr const PieceInfo& getPieceInfo(PieceType type) {
    return kPieceInfo[static_cast<uint8_t>(type)];
}
