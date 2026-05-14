#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <string_view>

class Piece;

/**
 * @brief Cooldown / charge tracking for abilities.
 */
enum class AbilityTrigger : uint8_t {
    Active    = 0,  // Manual activation by player
    OnHit     = 1,  // Triggers when the piece lands an attack
    OnAttacked= 2,  // Triggers when the piece is attacked
    OnDeath   = 3,  // Triggers when the piece would be defeated
    OnTurn    = 4,  // Triggers at the start of each turn
    Passive   = 5,  // Always active, no cooldown
};

/**
 * @brief A single ability that a piece can use.
 *
 * Abilities are the roguelike "skill system" on top of the chess moves.
 * Examples: Teleport, Fireball, Heal Wave, Summon Skeleton, etc.
 */
class Ability {
public:
    using EffectFn = std::function<bool(Piece& user)>;

    Ability() = default;
    Ability(std::string name, std::string description, AbilityTrigger trigger,
            int32_t cooldown, int32_t maxCharges, EffectFn effect);

    // ── Accessors ─────────────────────────────────────────────────────────────
    [[nodiscard]] std::string_view name()        const noexcept { return name_; }
    [[nodiscard]] std::string_view description() const noexcept { return description_; }
    [[nodiscard]] AbilityTrigger   trigger()     const noexcept { return trigger_; }
    [[nodiscard]] int32_t          cooldown()    const noexcept { return cooldown_; }
    [[nodiscard]] int32_t          maxCharges()  const noexcept { return maxCharges_; }
    [[nodiscard]] int32_t          currentCooldown() const noexcept { return currentCooldown_; }
    [[nodiscard]] int32_t          charges()     const noexcept { return charges_; }
    [[nodiscard]] bool             isReady()     const noexcept { return charges_ > 0 && currentCooldown_ == 0; }

    // ── Usage ─────────────────────────────────────────────────────────────────
    /// Attempt to activate the ability. Returns true if successful.
    [[nodiscard]] bool activate(Piece& user);

    /// Call at start of the piece's turn to tick cooldowns.
    void tickCooldown() noexcept;

    /// Reset ability state (full restore).
    void reset() noexcept;

private:
    std::string name_;
    std::string description_;
    AbilityTrigger trigger_     = AbilityTrigger::Active;
    int32_t        cooldown_    = 0;
    int32_t        maxCharges_  = 1;
    int32_t        currentCooldown_ = 0;
    int32_t        charges_         = 1;
    EffectFn       effect_       = nullptr;
};
