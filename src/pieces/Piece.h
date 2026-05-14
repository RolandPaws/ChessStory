#pragma once

#include "pieces/PieceType.h"
#include "player/Ability.h"   // for ability slots
#include "player/Inventory.h" // for items
#include "utils/Vector2D.h"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

/**
 * @brief Base class for all pieces on the chess board.
 *
 * Pieces have stats (HP, attack, defence), a movement range, abilities,
 * and an inventory.  This is the roguelike character system mounted on
 * a chess board.
 */
class Piece : public std::enable_shared_from_this<Piece> {
public:
    Piece(PieceType type, Faction faction, Vector2D position = {});

    virtual ~Piece() = default;

    // ── Accessors ─────────────────────────────────────────────────────────────
    [[nodiscard]] PieceType        type()        const noexcept { return type_; }
    [[nodiscard]] Faction          faction()     const noexcept { return faction_; }
    [[nodiscard]] const PieceInfo& info()        const noexcept { return getPieceInfo(type_); }
    [[nodiscard]] Vector2D         position()    const noexcept { return position_; }
    [[nodiscard]] std::string_view name()        const noexcept { return info().name; }

    // ── Stats ─────────────────────────────────────────────────────────────────
    [[nodiscard]] int32_t hp()           const noexcept { return hp_; }
    [[nodiscard]] int32_t maxHP()        const noexcept { return maxHP_; }
    [[nodiscard]] int32_t attack()       const noexcept { return attack_; }
    [[nodiscard]] int32_t defence()      const noexcept { return defence_; }
    [[nodiscard]] int32_t moveRange()    const noexcept { return moveRange_; }
    [[nodiscard]] int32_t level()        const noexcept { return level_; }
    [[nodiscard]] int32_t xp()           const noexcept { return xp_; }

    [[nodiscard]] bool    isAlive()      const noexcept { return hp_ > 0; }
    [[nodiscard]] bool    canFly()       const noexcept { return canFly_; }
    [[nodiscard]] bool    canPhase()     const noexcept { return canPhase_; }

    // ── Mutators ──────────────────────────────────────────────────────────────
    void setPosition(Vector2D pos) noexcept { position_ = pos; }

    void takeDamage(int32_t amount);
    void heal(int32_t amount);
    void addXP(int32_t amount);
    void levelUp();

    void setAttack(int32_t a)   noexcept { attack_ = a; }
    void setDefence(int32_t d)  noexcept { defence_ = d; }
    void setMoveRange(int32_t m) noexcept { moveRange_ = m; }

    // ── Abilities ─────────────────────────────────────────────────────────────
    [[nodiscard]] const std::vector<Ability>& abilities() const noexcept { return abilities_; }
    void addAbility(const Ability& ability);
    bool useAbility(size_t index);

    // ── Inventory ─────────────────────────────────────────────────────────────
    [[nodiscard]] Inventory&       inventory()       noexcept { return inventory_; }
    [[nodiscard]] const Inventory& inventory() const noexcept { return inventory_; }

    // ── Skills ────────────────────────────────────────────────────────────────
    [[nodiscard]] const std::vector<Skill>& skills() const noexcept { return skills_; }
    void addSkill(const Skill& skill);

    // ── Virtual hooks ─────────────────────────────────────────────────────────
    /// Called at the start of this piece's turn.
    virtual void onTurnStart() {}
    /// Called when this piece captures another.
    virtual void onCapture([[maybe_unused]] Piece& target) {}
    /// Called when this piece is captured.
    virtual void onDefeated() {}
    /// Called each turn for end-of-turn effects.
    virtual void onTurnEnd() {}

private:
    PieceType type_;
    Faction   faction_;
    Vector2D  position_;

    // Stats
    int32_t hp_         = 0;
    int32_t maxHP_      = 0;
    int32_t attack_     = 0;
    int32_t defence_    = 0;
    int32_t moveRange_  = 0;
    int32_t level_      = 1;
    int32_t xp_         = 0;

    // Flags
    bool canFly_   = false;
    bool canPhase_ = false;

    // Roguelike systems
    std::vector<Ability> abilities_;
    Inventory            inventory_;
    std::vector<Skill>   skills_;
};
