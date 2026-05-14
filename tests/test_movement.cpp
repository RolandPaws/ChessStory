#include "core/Board.h"
#include "pieces/Movement.h"
#include "pieces/Piece.h"
#include "pieces/PieceType.h"

#include <cassert>
#include <iostream>

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

    std::cout << "=== Movement Tests ===\n";

    // ── King movement ─────────────────────────────────────────────────────
    {
        TEST("King can move one step in any direction");
        Board board;
        Piece king(PieceType::King, Faction::Player, Vector2D{5, 5});
        auto moves = MovementSystem::getValidMoves(board, king, {5, 5});
        // 8 possible moves
        ASSERT(moves.size() == 8);
        END_TEST();
    }

    {
        TEST("King cannot move out of bounds");
        Board board(8);
        Piece king(PieceType::King, Faction::Player, Vector2D{0, 0});
        auto moves = MovementSystem::getValidMoves(board, king, {0, 0});
        // Corner: only 3 moves (1,0), (0,1), (1,1)
        ASSERT(moves.size() == 3);
        END_TEST();
    }

    // ── Knight movement ───────────────────────────────────────────────────
    {
        TEST("Knight has 8 L-shaped moves in open space");
        Board board(8);
        Piece knight(PieceType::Knight, Faction::Player, Vector2D{4, 4});
        auto moves = MovementSystem::getValidMoves(board, knight, {4, 4});
        ASSERT(moves.size() == 8);
        END_TEST();
    }

    {
        TEST("Knight near edge has fewer moves");
        Board board(8);
        Piece knight(PieceType::Knight, Faction::Player, Vector2D{0, 0});
        auto moves = MovementSystem::getValidMoves(board, knight, {0, 0});
        ASSERT(moves.size() == 2);  // (2,1) and (1,2)
        END_TEST();
    }

    // ── Queen movement ────────────────────────────────────────────────────
    {
        TEST("Queen has many moves on open board");
        Board board(8);
        Piece queen(PieceType::Queen, Faction::Player, Vector2D{4, 4});
        auto moves = MovementSystem::getValidMoves(board, queen, {4, 4});
        // Queen on centre of 8x8: 4+4+7+7 = 22 (but 27 with diagonals = 13+14)
        // Horizontal: 3 left + 3 right = 6
        // Vertical: 3 up + 3 down = 6
        // Diagonals: 4 directions with up to 3 steps each = 12
        // Total: 6+6+12 = 24
        ASSERT(moves.size() > 20);
        END_TEST();
    }

    // ── Blocked movement ──────────────────────────────────────────────────
    {
        TEST("Pieces block movement");
        Board board(8);
        Piece queen(PieceType::Queen, Faction::Player, Vector2D{4, 4});
        // Place a friendly piece blocking to the right
        auto blocker = std::make_shared<Piece>(PieceType::Pawn, Faction::Player, Vector2D{6, 4});
        board.placePiece(blocker, {6, 4});

        auto moves = MovementSystem::getValidMoves(board, queen, {4, 4});
        // Rightward moves should only go to 5,4 (blocked at 6)
        bool hasFiveFour = false;
        bool hasSixFour = false;
        for (const auto& m : moves) {
            if (m == Vector2D{5, 4}) hasFiveFour = true;
            if (m == Vector2D{6, 4}) hasSixFour = true;
        }
        ASSERT(hasFiveFour);
        ASSERT(!hasSixFour);
        END_TEST();
    }

    {
        TEST("Can capture enemy piece");
        Board board(8);
        Piece queen(PieceType::Queen, Faction::Player, Vector2D{4, 4});
        auto enemy = std::make_shared<Piece>(PieceType::Pawn, Faction::Enemy, Vector2D{6, 4});
        board.placePiece(enemy, {6, 4});

        auto moves = MovementSystem::getValidMoves(board, queen, {4, 4});
        bool canCapture = false;
        for (const auto& m : moves) {
            if (m == Vector2D{6, 4}) canCapture = true;
        }
        ASSERT(canCapture);
        END_TEST();
    }

    // ── Phantom phase movement ────────────────────────────────────────────
    {
        TEST("Phantom can move through walls");
        Board board(8);
        Piece phantom(PieceType::Phantom, Faction::Player, Vector2D{2, 2});
        board.setTileType({3, 2}, TileType::Wall);

        auto moves = MovementSystem::getValidMoves(board, phantom, {2, 2});
        bool canMoveToWall = false;
        for (const auto& m : moves) {
            if (m == Vector2D{3, 2}) canMoveToWall = true;
        }
        ASSERT(canMoveToWall);  // Phantom phases through
        END_TEST();
    }

    std::cout << "\n" << (failures == 0 ? "All tests passed!" : "Some tests failed!")
              << " (" << failures << " failures)\n";
    return failures;
}
