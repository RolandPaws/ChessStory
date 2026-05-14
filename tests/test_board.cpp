#include "core/Board.h"
#include "core/Tile.h"
#include "core/TileType.h"
#include "pieces/Piece.h"

#include <cassert>
#include <iostream>

// Simple test framework without external dependencies
#define TEST(name)                                    \
    do {                                              \
        std::cout << "  " << name << "... ";          \
        test_passed_ = true;                          \
    } while(0)

#define ASSERT(cond)                                  \
    do {                                              \
        if (!(cond)) {                                \
            std::cerr << "\nFAIL: " << #cond          \
                      << " (" << __FILE__ << ":"      \
                      << __LINE__ << ")\n";           \
            test_passed_ = false;                     \
        }                                             \
    } while(0)

#define END_TEST()                                    \
    do {                                              \
        std::cout << (test_passed_ ? "PASS" : "FAIL") << "\n"; \
        if (!test_passed_) ++failures;                \
    } while(0)

int main() {
    int failures = 0;
    bool test_passed_ = true;

    std::cout << "=== Board Tests ===\n";

    // ── Board creation ────────────────────────────────────────────────────
    {
        TEST("Default board size is 12x12");
        Board board;
        ASSERT(board.width() == 12);
        ASSERT(board.height() == 12);
        END_TEST();
    }

    {
        TEST("Custom board size");
        Board board(16);
        ASSERT(board.width() == 16);
        ASSERT(board.height() == 16);
        END_TEST();
    }

    {
        TEST("In-bounds detection");
        Board board;
        ASSERT(board.inBounds({0, 0}));
        ASSERT(board.inBounds({11, 11}));
        ASSERT(!board.inBounds({-1, 0}));
        ASSERT(!board.inBounds({0, 12}));
        ASSERT(!board.inBounds({12, 0}));
        END_TEST();
    }

    // ── Tile access ───────────────────────────────────────────────────────
    {
        TEST("Tile default type is Normal");
        Board board;
        ASSERT(board.at({3, 5}).type() == TileType::Normal);
        END_TEST();
    }

    {
        TEST("Set tile type");
        Board board;
        board.setTileType({5, 5}, TileType::Fire);
        ASSERT(board.at({5, 5}).type() == TileType::Fire);
        END_TEST();
    }

    {
        TEST("Set tile type out of bounds is safe");
        Board board;
        board.setTileType({99, 99}, TileType::Fire);  // should not crash
        END_TEST();
    }

    // ── Piece management ──────────────────────────────────────────────────
    {
        TEST("Place and retrieve piece");
        Board board;
        auto piece = std::make_shared<Piece>(PieceType::King, Faction::Player);
        board.placePiece(piece, {3, 3});
        ASSERT(board.pieceAt({3, 3}) == piece);
        ASSERT(board.pieceAt({4, 4}) == nullptr);
        END_TEST();
    }

    {
        TEST("Remove piece");
        Board board;
        auto piece = std::make_shared<Piece>(PieceType::Queen, Faction::Player);
        board.placePiece(piece, {5, 5});
        auto removed = board.removePiece({5, 5});
        ASSERT(removed == piece);
        ASSERT(board.pieceAt({5, 5}) == nullptr);
        END_TEST();
    }

    {
        TEST("Move piece");
        Board board;
        auto piece = std::make_shared<Piece>(PieceType::Knight, Faction::Player);
        board.placePiece(piece, {0, 0});
        board.movePiece({0, 0}, {2, 1});
        ASSERT(board.pieceAt({0, 0}) == nullptr);
        ASSERT(board.pieceAt({2, 1}) == piece);
        END_TEST();
    }

    // ── Board reset ───────────────────────────────────────────────────────
    {
        TEST("Reset board clears pieces and tiles");
        Board board;
        auto piece = std::make_shared<Piece>(PieceType::Pawn, Faction::Player);
        board.placePiece(piece, {1, 1});
        board.setTileType({2, 2}, TileType::Lava);
        board.reset();
        ASSERT(board.pieceAt({1, 1}) == nullptr);
        ASSERT(board.at({2, 2}).type() == TileType::Normal);
        END_TEST();
    }

    std::cout << "\n" << (failures == 0 ? "All tests passed!" : "Some tests failed!")
              << " (" << failures << " failures)\n";
    return failures;
}
