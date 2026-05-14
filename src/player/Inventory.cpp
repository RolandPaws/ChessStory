#include "player/Inventory.h"
#include "pieces/Piece.h"

// ── Item ──────────────────────────────────────────────────────────────────────

Item::Item(std::string name, std::string description, Category cat, Rarity rarity,
           int32_t attackBonus, int32_t defenceBonus, int32_t hpBonus)
    : name_(std::move(name))
    , description_(std::move(description))
    , category_(cat)
    , rarity_(rarity)
    , attackBonus_(attackBonus)
    , defenceBonus_(defenceBonus)
    , hpBonus_(hpBonus)
{
}

void Item::applyTo(Piece& piece) const {
    piece.setAttack(piece.attack() + attackBonus_);
    piece.setDefence(piece.defence() + defenceBonus_);
    // HP bonus applied via heal or maxHP increase as needed
    if (hpBonus_ > 0) {
        piece.heal(hpBonus_);
    }
}

void Item::removeFrom(Piece& piece) const {
    piece.setAttack(piece.attack() - attackBonus_);
    piece.setDefence(piece.defence() - defenceBonus_);
}

// ── Inventory ─────────────────────────────────────────────────────────────────

bool Inventory::addItem(const Item& item) {
    if (items_.size() >= kMaxItems) return false;
    items_.push_back(item);
    return true;
}

bool Inventory::removeItem(size_t index) {
    if (index >= items_.size()) return false;
    items_.erase(items_.begin() + static_cast<ptrdiff_t>(index));
    return true;
}

void Inventory::clear() noexcept {
    items_.clear();
}
