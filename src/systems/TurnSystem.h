#pragma once

#include <cstdint>
#include <memory>
#include <vector>

class Board;
class Player;
class Piece;

/**
 * @brief Manages the turn order for all players and pieces.
 *
 * The turn system supports:
 * - Multiple players (human and AI)
 * - Piece initiative order (not just "all of one player, then all of another")
 * - Extra actions from abilities / items / tiles
 * - Turn-phase callbacks
 */
class TurnSystem {
public:
    enum class Phase : uint8_t {
        StartOfTurn,    // Resource regen, passive effects, tile ticks
        Movement,       // Move pieces
        Action,         // Attack / use ability
        EndOfTurn,      // Cleanup, duration expire
        COUNT
    };

    explicit TurnSystem(std::vector<std::shared_ptr<Player>> players);

    // ── Turn flow ─────────────────────────────────────────────────────────────
    /// Advance to the next phase / piece / player.
    void nextPhase();

    /// Get the currently active entity.
    [[nodiscard]] std::shared_ptr<Piece> currentPiece() const noexcept { return currentPiece_; }

    [[nodiscard]] int32_t turnNumber() const noexcept { return turnNumber_; }
    [[nodiscard]] Phase   currentPhase() const noexcept { return currentPhase_; }

    /// Whether the current piece still has their action for this turn.
    [[nodiscard]] bool hasAction() const noexcept { return hasAction_; }
    void consumeAction() noexcept { hasAction_ = false; }

    /// Whether the current piece still has their move for this turn.
    [[nodiscard]] bool hasMove() const noexcept { return hasMove_; }
    void consumeMove() noexcept { hasMove_ = false; }

    // ── Queries ───────────────────────────────────────────────────────────────
    [[nodiscard]] bool isGameOver() const noexcept { return gameOver_; }
    [[nodiscard]] std::shared_ptr<Player> winner() const noexcept { return winner_; }

    /// Check if any player has been defeated.
    void checkVictoryConditions();

private:
    std::vector<std::shared_ptr<Player>> players_;
    size_t currentPlayerIndex_ = 0;
    size_t currentPieceIndex_  = 0;
    int32_t turnNumber_ = 1;

    Phase phase_ = Phase::StartOfTurn;
    std::shared_ptr<Piece> currentPiece_;
    bool hasMove_   = true;
    bool hasAction_ = true;
    bool gameOver_  = false;
    std::shared_ptr<Player> winner_;

    void advanceToNextAlivePiece();
    void startNewTurn();
};
