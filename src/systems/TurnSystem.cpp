#include "systems/TurnSystem.h"
#include "core/Board.h"
#include "player/Player.h"

#include <algorithm>

TurnSystem::TurnSystem(std::vector<std::shared_ptr<Player>> players)
    : players_(std::move(players))
{
    if (!players_.empty()) {
        advanceToNextAlivePiece();
    }
}

void TurnSystem::nextPhase() {
    if (gameOver_) return;

    switch (phase_) {
    case Phase::StartOfTurn:
        // Move to movement phase
        phase_ = Phase::Movement;
        hasMove_ = true;
        hasAction_ = false;  // action comes after move
        break;

    case Phase::Movement:
        // Move to action phase
        phase_ = Phase::Action;
        hasMove_ = false;
        hasAction_ = true;
        break;

    case Phase::Action:
        // End of this piece's turn
        if (currentPiece_) {
            currentPiece_->onTurnEnd();
        }
        phase_ = Phase::EndOfTurn;
        break;

    case Phase::EndOfTurn:
        // Advance to next piece
        ++currentPieceIndex_;
        advanceToNextAlivePiece();
        if (currentPiece_) {
            phase_ = Phase::StartOfTurn;
            // Check if we wrapped to a new turn
            if (currentPieceIndex_ == 0) {
                startNewTurn();
            }
        }
        break;

    default:
        break;
    }
}

void TurnSystem::advanceToNextAlivePiece() {
    // Iterate through players and their pieces to find the next alive one
    const size_t totalPlayers = players_.size();
    for (size_t p = 0; p < totalPlayers; ++p) {
        const auto& player = players_[(currentPlayerIndex_ + p) % totalPlayers];
        const auto& army = player->army();

        for (size_t i = 0; i < army.size(); ++i) {
            const auto& piece = army[(currentPieceIndex_ + i) % army.size()];
            if (piece && piece->isAlive()) {
                currentPiece_ = piece;
                currentPieceIndex_ = (currentPieceIndex_ + i) % army.size();
                currentPlayerIndex_ = (currentPlayerIndex_ + p) % totalPlayers;
                return;
            }
        }
    }

    // No alive pieces found anywhere
    currentPiece_.reset();
    checkVictoryConditions();
}

void TurnSystem::startNewTurn() {
    ++turnNumber_;
    for (auto& player : players_) {
        player->onTurnStart();
    }
    // Board evolution happens here (called externally via Game)
}

void TurnSystem::checkVictoryConditions() {
    for (auto& player : players_) {
        if (player->isDefeated()) {
            gameOver_ = true;
            // Find the first non-defeated player as winner
            for (auto& other : players_) {
                if (other != player && !other->isDefeated()) {
                    winner_ = other;
                    return;
                }
            }
        }
    }
}
