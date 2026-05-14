#pragma once

#include <cstdint>
#include <memory>

class Board;
class Piece;

/**
 * @brief Handles all combat interactions between pieces.
 *
 * Supports:
 * - Melee and ranged attacks
 * - Damage calculation (attack vs defence)
 * - Critical hits
 * - Elemental interactions (tile-based damage modifiers)
 * - Lifesteal, thorns, and other skill effects
 * - Ability triggers on hit/attacked
 */
class CombatSystem {
public:
    struct CombatResult {
        int32_t damageDealt      = 0;
        int32_t damageReceived   = 0;   // from thorns / counter
        bool    targetDefeated   = false;
        bool    attackerDefeated = false;
        bool    criticalHit      = false;
    };

    /// Resolve a melee attack from `attacker` to `defender`.
    static CombatResult resolveMeleeAttack(const Board& board,
                                           Piece& attacker,
                                           Piece& defender);

    /// Resolve a ranged attack from `attacker` to `defender`.
    static CombatResult resolveRangedAttack(const Board& board,
                                            Piece& attacker,
                                            Piece& defender);

    /// Check if a piece can attack a target (range, line-of-sight, etc.)
    [[nodiscard]] static bool canAttack(const Board& board,
                                        const Piece& attacker,
                                        const Piece& defender,
                                        int32_t range);

private:
    static CombatResult resolveAttack(const Board& board,
                                      Piece& attacker,
                                      Piece& defender,
                                      bool isRanged);
};
