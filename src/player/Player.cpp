#include "player/Player.h"

#include <algorithm>

Player::Player(std::string name, Faction faction)
    : name_(std::move(name))
    , faction_(faction)
{
}

void Player::addPiece(std::shared_ptr<Piece> piece) {
    army_.push_back(std::move(piece));
}

void Player::removePiece(const Piece& piece) {
    auto it = std::remove_if(army_.begin(), army_.end(),
        [&](const auto& p) { return p.get() == &piece; });
    army_.erase(it, army_.end());
}

bool Player::isDefeated() const noexcept {
    // The player is defeated if the commander is dead or all pieces are dead
    if (!commander_ || !commander_->isAlive()) return true;
    return std::none_of(army_.begin(), army_.end(),
                        [](const auto& p) { return p->isAlive(); });
}

bool Player::spendGold(int32_t amount) noexcept {
    if (gold_ < amount) return false;
    gold_ -= amount;
    return true;
}

bool Player::spendMana(int32_t amount) noexcept {
    if (mana_ < amount) return false;
    mana_ -= amount;
    return true;
}

void Player::onTurnStart() {
    // Passive resource regeneration
    addMana(10);
    addGold(5);

    // Tick ability cooldowns for all pieces
    for (auto& piece : army_) {
        if (piece && piece->isAlive()) {
            piece->onTurnStart();
            for (auto& ability : piece->abilities()) {
                // const_cast because tickCooldown is logical mutability
                const_cast<Ability&>(ability).tickCooldown();
            }
        }
    }
}
