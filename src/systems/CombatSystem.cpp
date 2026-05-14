#include "systems/CombatSystem.h"
#include "core/Board.h"
#include "pieces/Piece.h"

#include <algorithm>
#include <cmath>
#include <random>

CombatResult CombatSystem::resolveMeleeAttack(const Board& board,
                                               Piece& attacker,
                                               Piece& defender) {
    return resolveAttack(board, attacker, defender, false);
}

CombatResult CombatSystem::resolveRangedAttack(const Board& board,
                                                Piece& attacker,
                                                Piece& defender) {
    return resolveAttack(board, attacker, defender, true);
}

bool CombatSystem::canAttack(const Board& board,
                              const Piece& attacker,
                              const Piece& defender,
                              int32_t range) {
    Vector2D delta = defender.position() - attacker.position();
    int32_t distance = delta.chebyshev();

    if (distance > range) return false;
    if (attacker.faction() == defender.faction()) return false;
    if (!defender.isAlive()) return false;

    // Simple line-of-sight check for ranged (ensure no blocking pieces)
    // TODO: proper LOS using Bresenham
    return true;
}

CombatResult CombatSystem::resolveAttack(const Board& board,
                                          Piece& attacker,
                                          Piece& defender,
                                          bool isRanged) {
    CombatResult result;

    // ── Critical hit check (10% base chance) ──────────────────────────────
    static std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> critDist(1, 100);
    result.criticalHit = critDist(rng) <= 10;

    // ── Damage calculation ────────────────────────────────────────────────
    int32_t baseDamage = attacker.attack();
    if (result.criticalHit) {
        baseDamage = static_cast<int32_t>(std::round(baseDamage * 1.5));
    }

    // Tile modifiers
    const auto& defenderTile = board.at(defender.position());
    if (defenderTile.type() == TileType::Empowered) {
        baseDamage += 2;
    }

    result.damageDealt = std::max(1, baseDamage - defender.defence());

    // ── Apply damage ──────────────────────────────────────────────────────
    defender.takeDamage(result.damageDealt);

    if (!defender.isAlive()) {
        result.targetDefeated = true;
        defender.onDefeated();
        attacker.onCapture(defender);
    }

    // ── Thorns / counter-attack (simplified) ──────────────────────────────
    // Melee attackers receive 1 thorns damage if defender has thorns
    if (!isRanged) {
        for (const auto& skill : defender.skills()) {
            if (skill.modifier() == Skill::Modifier::Thorns) {
                result.damageReceived = skill.value();
                attacker.takeDamage(result.damageReceived);
                if (!attacker.isAlive()) {
                    result.attackerDefeated = true;
                }
                break;
            }
        }
    }

    return result;
}
