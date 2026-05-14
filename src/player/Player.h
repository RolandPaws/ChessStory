#pragma once

#include "pieces/Piece.h"
#include "pieces/PieceType.h"

#include <cstdint>
#include <memory>
#include <vector>

/**
 * @brief Represents a human (or AI) player controlling a team of pieces.
 *
 * The player has a "commander" piece (like the King in classical chess)
 * and an army of other pieces.  The player also has global resources
 * (gold, mana) that persist across turns and can be used for abilities
 * or purchases.
 */
class Player {
public:
    explicit Player(std::string name, Faction faction);

    // ── Identity ──────────────────────────────────────────────────────────────
    [[nodiscard]] std::string_view name()    const noexcept { return name_; }
    [[nodiscard]] Faction          faction() const noexcept { return faction_; }

    // ── Army ──────────────────────────────────────────────────────────────────
    [[nodiscard]] const std::vector<std::shared_ptr<Piece>>& army() const noexcept { return army_; }
    void addPiece(std::shared_ptr<Piece> piece);
    void removePiece(const Piece& piece);

    [[nodiscard]] std::shared_ptr<Piece> commander() const noexcept { return commander_; }
    void setCommander(std::shared_ptr<Piece> piece) noexcept { commander_ = std::move(piece); }

    [[nodiscard]] bool isDefeated() const noexcept;

    // ── Resources ─────────────────────────────────────────────────────────────
    [[nodiscard]] int32_t gold() const noexcept { return gold_; }
    [[nodiscard]] int32_t mana() const noexcept { return mana_; }

    void addGold(int32_t amount) noexcept { gold_ += amount; }
    bool spendGold(int32_t amount) noexcept;
    void addMana(int32_t amount) noexcept { mana_ += amount; }
    bool spendMana(int32_t amount) noexcept;

    /// Called each turn to regenerate resources.
    void onTurnStart();

private:
    std::string name_;
    Faction     faction_ = Faction::Player;

    std::vector<std::shared_ptr<Piece>> army_;
    std::shared_ptr<Piece>              commander_;

    int32_t gold_ = 100;
    int32_t mana_ = 50;
};
