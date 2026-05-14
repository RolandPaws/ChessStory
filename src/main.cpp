#include "core/Game.h"

#include <iostream>
#include <chrono>
#include <thread>

/**
 * @brief Entry point for Chess Roguelike.
 *
 * Initialises the game, runs a simple console loop (placeholder for future
 * graphical UI or full terminal UI), and cleans up on exit.
 */
int main() {
    try {
        Game game;
        game.initialize();

        std::cout << "\nGame initialised with " << game.board().width()
                  << "x" << game.board().height() << " board.\n";
        std::cout << "Players: ";
        for (const auto& p : game.players()) {
            std::cout << p->name() << " (" << p->army().size() << " pieces) ";
        }
        std::cout << "\n\n";

        // ── Main loop (placeholder) ───────────────────────────────────────
        // In a real implementation this would be event-driven.  Here we
        // run a fixed number of auto-advance steps for demonstration.
        int32_t step = 0;
        constexpr int32_t kMaxDemoSteps = 10;

        while (game.isRunning() && step < kMaxDemoSteps) {
            game.update();
            if (game.isRunning()) {
                auto& turns = game.turns();
                std::cout << "Turn " << turns.turnNumber()
                          << " | Phase: " << static_cast<int>(turns.currentPhase());
                auto piece = turns.currentPiece();
                if (piece) {
                    std::cout << " | Active: " << piece->name()
                              << " @(" << piece->position().x << ","
                              << piece->position().y << ")";
                }
                std::cout << "\n";
            }
            ++step;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        if (!game.isGameOver()) {
            std::cout << "\nDemo complete after " << kMaxDemoSteps << " steps.\n";
        }

        std::cout << "Goodbye!\n";

    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
