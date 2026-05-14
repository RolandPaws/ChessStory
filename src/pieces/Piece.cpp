#include "pieces/Piece.h"

#include <algorithm>
#include <cassert>

Piece::Piece(PieceType type, Faction faction, Vector2D position)
    : type_(type)
    , faction_(faction)
    , position_(position)
{
    const auto& pi = info();
    hp_         = pi.baseHP;
    maxHP_      = pi.baseHP;
    attack_     = pi.baseAttack;
    defence_    = pi.baseDefence;
    moveRange_  = pi.baseMoveRange;
    canFly_     = pi.canFly;
    canPhase_   = pi.canPhase;
}

// ── Damage / healing ──────────────────────────────────────────────────────────

void Piece::takeDamage(int32_t amount) {
    int32_t effective = std::max(1, amount - defence_);  // minimum 1 damage
    hp_ = std::max(0, hp_ - effective);
}

void Piece::heal(int32_t amount) {
    hp_ = std::min(maxHP_, hp_ + amount);
}

// ── Progression ───────────────────────────────────────────────────────────────

void Piece::addXP(int32_t amount) {
    xp_ += amount;
    constexpr int32_t kXPPerLevel = 20;
    while (xp_ >= level_ * kXPPerLevel) {
        xp_ -= level_ * kXPPerLevel;
        levelUp();
    }
}

void Piece::levelUp() {
    ++level_;
    // Stat growth
    maxHP_   += 2;
    hp_       = maxHP_;   // full heal on level-up
    attack_  += 1;
    defence_ += 1;
    if (level_ % 3 == 0) {
        ++moveRange_;     // every 3 levels get +1 move
    }
}

// ── Abilities ─────────────────────────────────────────────────────────────────

void Piece::addAbility(const Ability& ability) {
    abilities_.push_back(ability);
}

bool Piece::useAbility(size_t index) {
    if (index >= abilities_.size()) return false;
    return abilities_[index].activate(*this);
}

// ── Skills ────────────────────────────────────────────────────────────────────

void Piece::addSkill(const Skill& skill) {
    skills_.push_back(skill);
}
