#pragma once

#include <cstdint>
#include <string>
#include <string_view>

class Piece;

/**
 * @brief Passive skill that modifies a piece's stats or behaviour.
 *
 * Skills are permanent (or at least persistent) upgrades gained through
 * level-ups, events, or items.  Unlike abilities, they don't need
 * activation — they are always on.
 *
 * Examples: "+2 Attack", "Ignore terrain penalties", "Counter-attack"
 */
class Skill {
public:
    enum class Modifier : uint8_t {
        AttackBonus,
        DefenceBonus,
        HPBonus,
        MoveRangeBonus,
        Lifesteal,          // heal for % of damage dealt
        Thorns,             // reflect damage
        Flying,
        Phasing,
        ExtraAction,        // chance for extra move per turn
        TileImmunity,       // immune to tile effects
        COUNT
    };

    Skill() = default;
    Skill(std::string name, std::string description, Modifier mod, int32_t value);

    [[nodiscard]] std::string_view name()        const noexcept { return name_; }
    [[nodiscard]] std::string_view description() const noexcept { return description_; }
    [[nodiscard]] Modifier         modifier()    const noexcept { return modifier_; }
    [[nodiscard]] int32_t          value()       const noexcept { return value_; }

    /// Apply this skill's effects to the given piece.
    void applyTo(Piece& piece) const;

private:
    std::string name_;
    std::string description_;
    Modifier    modifier_ = Modifier::AttackBonus;
    int32_t     value_    = 0;
};
