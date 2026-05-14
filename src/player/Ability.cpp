#include "player/Ability.h"
#include "pieces/Piece.h"

Ability::Ability(std::string name, std::string description, AbilityTrigger trigger,
                 int32_t cooldown, int32_t maxCharges, EffectFn effect)
    : name_(std::move(name))
    , description_(std::move(description))
    , trigger_(trigger)
    , cooldown_(cooldown)
    , maxCharges_(maxCharges)
    , charges_(maxCharges)
    , effect_(std::move(effect))
{
}

bool Ability::activate(Piece& user) {
    if (!isReady()) return false;
    if (!effect_) return false;

    if (effect_(user)) {
        --charges_;
        currentCooldown_ = cooldown_;
        return true;
    }
    return false;
}

void Ability::tickCooldown() noexcept {
    if (currentCooldown_ > 0) {
        --currentCooldown_;
    }
    // Restore a charge when cooldown expires and we're below max
    if (currentCooldown_ == 0 && charges_ < maxCharges_) {
        ++charges_;
    }
}

void Ability::reset() noexcept {
    charges_ = maxCharges_;
    currentCooldown_ = 0;
}
