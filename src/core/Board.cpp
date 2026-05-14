#include "core/Board.h"

// ── Construction ──────────────────────────────────────────────────────────────

Board::Board(int32_t size)
    : width_(size)
    , height_(size)
    , grid_(static_cast<size_t>(size) * static_cast<size_t>(size))
{
    // Initialise grid positions
    for (int32_t y = 0; y < height_; ++y) {
        for (int32_t x = 0; x < width_; ++x) {
            auto& tile = grid_[index({x, y})];
            tile.setPosition({x, y});
            tile.setType(TileType::Normal);
        }
    }
    generateStandardLayout();
}

// ── Grid access ───────────────────────────────────────────────────────────────

Tile& Board::at(Vector2D pos) {
    return grid_[index(pos)];
}

const Tile& Board::at(Vector2D pos) const {
    return grid_[index(pos)];
}

bool Board::inBounds(Vector2D pos) const noexcept {
    return pos.x >= 0 && pos.x < width_ && pos.y >= 0 && pos.y < height_;
}

// ── Piece management ──────────────────────────────────────────────────────────

std::shared_ptr<Piece> Board::pieceAt(Vector2D pos) const {
    auto it = pieces_.find(pos);
    return (it != pieces_.end()) ? it->second : nullptr;
}

void Board::placePiece(std::shared_ptr<Piece> piece, Vector2D pos) {
    pieces_[pos] = std::move(piece);
}

std::shared_ptr<Piece> Board::removePiece(Vector2D pos) {
    auto it = pieces_.find(pos);
    if (it == pieces_.end()) return nullptr;
    auto piece = std::move(it->second);
    pieces_.erase(it);
    return piece;
}

void Board::movePiece(Vector2D from, Vector2D to) {
    auto piece = removePiece(from);
    if (piece) {
        placePiece(std::move(piece), to);
    }
}

std::vector<std::shared_ptr<Piece>> Board::allPieces() const {
    std::vector<std::shared_ptr<Piece>> result;
    result.reserve(pieces_.size());
    for (const auto& [_, piece] : pieces_) {
        result.push_back(piece);
    }
    return result;
}

// ── Tile mutation ─────────────────────────────────────────────────────────────

void Board::setTileType(Vector2D pos, TileType type) {
    if (inBounds(pos)) {
        at(pos).setType(type);
    }
}

void Board::addTemporaryTile(Vector2D pos, TileType type, int32_t duration) {
    if (inBounds(pos)) {
        at(pos).setType(type);
        at(pos).setRemainingTurns(duration);
    }
}

void Board::evolve() {
    // Decrement temporary tile durations and revert expired ones
    for (int32_t y = 0; y < height_; ++y) {
        for (int32_t x = 0; x < width_; ++x) {
            auto& tile = at({x, y});
            if (tile.remainingTurns() > 0) {
                tile.decrementTurns();
                if (tile.remainingTurns() == 0) {
                    tile.setType(TileType::Normal);
                }
            }
        }
    }

    // TODO: random board-evolution events (earthquakes, tile spread, etc.)
}

void Board::reset() {
    pieces_.clear();
    for (auto& tile : grid_) {
        tile.setType(TileType::Normal);
        tile.clearWarpTarget();
        tile.setRemainingTurns(-1);
    }
    generateStandardLayout();
}

// ── Helpers ───────────────────────────────────────────────────────────────────

int32_t Board::index(Vector2D pos) const noexcept {
    return pos.y * width_ + pos.x;
}

void Board::generateStandardLayout() {
    // Place some special tiles in the inner region as a demo
    for (int32_t y = 2; y < height_ - 2; ++y) {
        for (int32_t x = 2; x < width_ - 2; ++x) {
            if ((x + y) % 7 == 0) {
                setTileType({x, y}, TileType::Healing);
            } else if ((x * y) % 13 == 0) {
                setTileType({x, y}, TileType::ManaWell);
            }
        }
    }
}
