#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

class Piece;

/**
 * @brief An equippable or consumable item.
 *
 * Items are roguelike loot that can be found on the board, dropped by
 * enemies, or crafted.  They provide stat bonuses, new abilities, or
 * temporary effects.
 */
class Item {
public:
    enum class Category : uint8_t {
        Weapon,       // boosts attack
        Armour,       // boosts defence
        Ring,         // special effects
        Consumable,   // one-time use (health potion, etc.)
        Artifact,     // unique powerful item
        KeyItem,      // story progression
    };

    enum class Rarity : uint8_t {
        Common    = 0,
        Uncommon  = 1,
        Rare      = 2,
        Epic      = 3,
        Legendary = 4,
    };

    Item() = default;
    Item(std::string name, std::string description, Category cat, Rarity rarity,
         int32_t attackBonus, int32_t defenceBonus, int32_t hpBonus);

    [[nodiscard]] std::string_view name()        const noexcept { return name_; }
    [[nodiscard]] std::string_view description() const noexcept { return description_; }
    [[nodiscard]] Category         category()    const noexcept { return category_; }
    [[nodiscard]] Rarity           rarity()      const noexcept { return rarity_; }
    [[nodiscard]] int32_t          attackBonus() const noexcept { return attackBonus_; }
    [[nodiscard]] int32_t          defenceBonus()const noexcept { return defenceBonus_; }
    [[nodiscard]] int32_t          hpBonus()     const noexcept { return hpBonus_; }

    /// Apply item effects to a piece.
    void applyTo(Piece& piece) const;

    /// Remove item effects from a piece.
    void removeFrom(Piece& piece) const;

private:
    std::string name_;
    std::string description_;
    Category    category_   = Category::Consumable;
    Rarity      rarity_     = Rarity::Common;
    int32_t     attackBonus_  = 0;
    int32_t     defenceBonus_ = 0;
    int32_t     hpBonus_      = 0;
};

/**
 * @brief A piece's inventory (holds items).
 */
class Inventory {
public:
    static constexpr size_t kMaxItems = 8;

    [[nodiscard]] const std::vector<Item>& items() const noexcept { return items_; }

    bool addItem(const Item& item);
    bool removeItem(size_t index);
    void clear() noexcept;

private:
    std::vector<Item> items_;
};
