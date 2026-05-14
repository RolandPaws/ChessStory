#include "core/Game.h"

#include "pieces/Movement.h"
#include "systems/CombatSystem.h"

#include <iostream>
#include <string>

Game::Game() {
    board_ = std::make_shared<Board>(Board::kDefaultSize);
}

void Game::initialize() {
    std::cout << "=== Chess Roguelike v0.1 ===\n";
    std::cout << "Board: " << board_->width() << "x" << board_->height() << "\n\n";

    setupPlayers();
    setupPieces();

    turnSystem_ = std::make_unique<TurnSystem>(players_);
    boardEvolution_ = std::make_unique<BoardEvolution>(board_);

    running_ = true;
}

void Game::update() {
    if (!running_ || !turnSystem_) return;

    // ── Phase progression ─────────────────────────────────────────────────
    turnSystem_->nextPhase();

    // ── Board evolution (at EndOfTurn phase) ──────────────────────────────
    if (turnSystem_->currentPhase() == TurnSystem::Phase::EndOfTurn) {
        boardEvolution_->evolve(turnSystem_->turnNumber());
    }

    // ── Victory check ─────────────────────────────────────────────────────
    if (turnSystem_->isGameOver()) {
        auto winner = turnSystem_->winner();
        if (winner) {
            std::cout << "Game Over! " << winner->name() << " wins!\n";
        } else {
            std::cout << "Game Over! Draw.\n";
        }
        running_ = false;
    }
}

// ── Setup ─────────────────────────────────────────────────────────────────────

void Game::setupPlayers() {
    auto human = std::make_shared<Player>("Hero", Faction::Player);
    auto enemy = std::make_shared<Player>("Dark Lord", Faction::Enemy);
    players_.push_back(std::move(human));
    players_.push_back(std::move(enemy));
}

void Game::setupPieces() {
    if (players_.size() < 2) return;

    auto& human = players_[0];
    auto& enemy = players_[1];

    // ── Player army ───────────────────────────────────────────────────────
    auto king = std::make_shared<Piece>(PieceType::King, Faction::Player, Vector2D{5, 0});
    human->setCommander(king);
    human->addPiece(king);
    board_->placePiece(king, {5, 0});

    auto queen = std::make_shared<Piece>(PieceType::Queen, Faction::Player, Vector2D{4, 0});
    human->addPiece(queen);
    board_->placePiece(queen, {4, 0});

    // Rooks
    auto rook1 = std::make_shared<Piece>(PieceType::Rook, Faction::Player, Vector2D{0, 0});
    auto rook2 = std::make_shared<Piece>(PieceType::Rook, Faction::Player, Vector2D{11, 0});
    human->addPiece(rook1); board_->placePiece(rook1, {0, 0});
    human->addPiece(rook2); board_->placePiece(rook2, {11, 0});

    // Knights
    auto knight1 = std::make_shared<Piece>(PieceType::Knight, Faction::Player, Vector2D{1, 0});
    auto knight2 = std::make_shared<Piece>(PieceType::Knight, Faction::Player, Vector2D{10, 0});
    human->addPiece(knight1); board_->placePiece(knight1, {1, 0});
    human->addPiece(knight2); board_->placePiece(knight2, {10, 0});

    // Bishops
    auto bishop1 = std::make_shared<Piece>(PieceType::Bishop, Faction::Player, Vector2D{2, 0});
    auto bishop2 = std::make_shared<Piece>(PieceType::Bishop, Faction::Player, Vector2D{9, 0});
    human->addPiece(bishop1); board_->placePiece(bishop1, {2, 0});
    human->addPiece(bishop2); board_->placePiece(bishop2, {9, 0});

    // Pawns (row 1)
    for (int32_t x = 0; x < board_->width(); ++x) {
        auto pawn = std::make_shared<Piece>(PieceType::Pawn, Faction::Player, Vector2D{x, 1});
        human->addPiece(pawn);
        board_->placePiece(pawn, {x, 1});
    }

    // Extended pieces
    auto archer = std::make_shared<Piece>(PieceType::Archer, Faction::Player, Vector2D{3, 0});
    human->addPiece(archer); board_->placePiece(archer, {3, 0});

    auto mage = std::make_shared<Piece>(PieceType::Mage, Faction::Player, Vector2D{8, 0});
    human->addPiece(mage); board_->placePiece(mage, {8, 0});

    mage->addAbility(Ability("Fireball", "Deals 3 damage to target enemy",
                              AbilityTrigger::Active, 2, 3,
                              [](Piece& user) -> bool {
                                  std::cout << user.name() << " casts Fireball!\n";
                                  return true;
                              }));


    // ── Enemy army (bottom of the board) ──────────────────────────────────
    int32_t bottomY = board_->height() - 1;

    auto enemyKing = std::make_shared<Piece>(PieceType::King, Faction::Enemy, Vector2D{5, bottomY});
    enemy->setCommander(enemyKing);
    enemy->addPiece(enemyKing);
    board_->placePiece(enemyKing, {5, bottomY});

    auto enemyQueen = std::make_shared<Piece>(PieceType::Queen, Faction::Enemy, Vector2D{4, bottomY});
    enemy->addPiece(enemyQueen);
    board_->placePiece(enemyQueen, {4, bottomY});

    // Enemy rooks
    auto erook1 = std::make_shared<Piece>(PieceType::Rook, Faction::Enemy, Vector2D{0, bottomY});
    auto erook2 = std::make_shared<Piece>(PieceType::Rook, Faction::Enemy, Vector2D{11, bottomY});
    enemy->addPiece(erook1); board_->placePiece(erook1, {0, bottomY});
    enemy->addPiece(erook2); board_->placePiece(erook2, {11, bottomY});

    // Enemy knights
    auto eknight1 = std::make_shared<Piece>(PieceType::Knight, Faction::Enemy, Vector2D{1, bottomY});
    auto eknight2 = std::make_shared<Piece>(PieceType::Knight, Faction::Enemy, Vector2D{10, bottomY});
    enemy->addPiece(eknight1); board_->placePiece(eknight1, {1, bottomY});
    enemy->addPiece(eknight2); board_->placePiece(eknight2, {10, bottomY});

    // Enemy bishops
    auto ebishop1 = std::make_shared<Piece>(PieceType::Bishop, Faction::Enemy, Vector2D{2, bottomY});
    auto ebishop2 = std::make_shared<Piece>(PieceType::Bishop, Faction::Enemy, Vector2D{9, bottomY});
    enemy->addPiece(ebishop1); board_->placePiece(ebishop1, {2, bottomY});
    enemy->addPiece(ebishop2); board_->placePiece(ebishop2, {9, bottomY});

    // Enemy pawns (row bottomY - 1)
    for (int32_t x = 0; x < board_->width(); ++x) {
        auto pawn = std::make_shared<Piece>(PieceType::Pawn, Faction::Enemy, Vector2D{x, bottomY - 1});
        enemy->addPiece(pawn);
        board_->placePiece(pawn, {x, bottomY - 1});
    }

    // Enemy extended pieces
    auto necro = std::make_shared<Piece>(PieceType::Necromancer, Faction::Enemy, Vector2D{3, bottomY});
    enemy->addPiece(necro); board_->placePiece(necro, {3, bottomY});

    auto valk = std::make_shared<Piece>(PieceType::Valkyrie, Faction::Enemy, Vector2D{8, bottomY});
    enemy->addPiece(valk); board_->placePiece(valk, {8, bottomY});
}
