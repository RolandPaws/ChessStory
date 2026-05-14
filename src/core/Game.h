#pragma once

#include "core/Board.h"
#include "player/Player.h"
#include "systems/TurnSystem.h"
#include "systems/BoardEvolution.h"

#include <memory>

/**
 * @brief Top-level game orchestrator.
 *
 * Owns the board, players, turn system, and board evolution.
 * Provides the main game loop and high-level API for the UI layer.
 */
class Game {
public:
    Game();
    ~Game() = default;

    // Disable copy / enable move
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) = default;
    Game& operator=(Game&&) = default;

    // ── Lifecycle ─────────────────────────────────────────────────────────────
    void initialize();
    void update();
    [[nodiscard]] bool isRunning() const noexcept { return running_; }
    void quit() noexcept { running_ = false; }

    // ── Accessors ─────────────────────────────────────────────────────────────
    [[nodiscard]] Board&                      board()       noexcept { return *board_; }
    [[nodiscard]] const Board&                board() const noexcept { return *board_; }
    [[nodiscard]] TurnSystem&                 turns()       noexcept { return *turnSystem_; }
    [[nodiscard]] const TurnSystem&           turns() const noexcept { return *turnSystem_; }
    [[nodiscard]] const std::vector<std::shared_ptr<Player>>& players() const noexcept { return players_; }

    // ── Game state ────────────────────────────────────────────────────────────
    [[nodiscard]] bool isGameOver() const noexcept { return turnSystem_ && turnSystem_->isGameOver(); }

private:
    std::shared_ptr<Board>              board_;
    std::vector<std::shared_ptr<Player>> players_;
    std::unique_ptr<TurnSystem>          turnSystem_;
    std::unique_ptr<BoardEvolution>      boardEvolution_;
    bool running_ = false;

    void setupPlayers();
    void setupPieces();
};
