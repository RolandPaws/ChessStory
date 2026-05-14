#include "systems/BoardEvolution.h"
#include "core/Board.h"
#include "core/TileType.h"

#include <algorithm>
#include <random>
#include <vector>

BoardEvolution::BoardEvolution(std::shared_ptr<Board> board)
    : board_(std::move(board))
{
}

void BoardEvolution::evolve(int32_t turnNumber) {
    if (turnNumber % frequency_ != 0) return;

    // Always process tile durations
    board_->evolve();

    // Random events based on turn number
    static std::mt19937 rng{std::random_device{}()};

    // As turns progress, events become more frequent/intense
    std::uniform_int_distribution<int> eventDist(0, 100);

    int roll = eventDist(rng);
    if (roll < 20) {
        triggerSpread(TileType::Fire);
    } else if (roll < 35) {
        triggerSpread(TileType::Thorn);
    } else if (roll < 45) {
        meltIceTiles();
    } else if (roll < 55) {
        triggerRandomGrowth();
    } else if (roll < 65) {
        randomTileMutation();
    } else if (roll < 75) {
        triggerEarthquake();
    }
    // 25% chance nothing happens
}

void BoardEvolution::triggerSpread(TileType type) {
    spreadTileType(type, 1);
}

void BoardEvolution::triggerEarthquake() {
    if (!board_) return;

    static std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> axisDist(0, 1);   // 0 = row, 1 = column
    std::uniform_int_distribution<int> idxDist(0, board_->height() - 1);

    int axis = axisDist(rng);
    int index = idxDist(rng);

    // Shift the row or column: tiles to the right/down get replaced
    if (axis == 0) {
        // Shift a row
        for (int32_t x = board_->width() - 1; x > 0; --x) {
            auto& tile = board_->at({x, index});
            tile.setType(board_->at({x - 1, index}).type());
        }
        board_->at({0, index}).setType(TileType::Wall);  // edge collapses
    } else {
        // Shift a column
        for (int32_t y = board_->height() - 1; y > 0; --y) {
            auto& tile = board_->at({index, y});
            tile.setType(board_->at({index, y - 1}).type());
        }
        board_->at({index, 0}).setType(TileType::Wall);
    }
}

void BoardEvolution::triggerRandomGrowth() {
    if (!board_) return;

    static std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> xDist(0, board_->width() - 1);
    std::uniform_int_distribution<int> yDist(0, board_->height() - 1);
    std::uniform_int_distribution<int> typeDist(3, 7);  // Fire, Ice, Thorn, Healing

    Vector2D pos{xDist(rng), yDist(rng)};
    if (board_->inBounds(pos) && board_->at(pos).type() == TileType::Normal) {
        board_->setTileType(pos, static_cast<TileType>(typeDist(rng)));
    }
}

void BoardEvolution::spreadTileType(TileType type, int32_t radius) {
    if (!board_) return;

    static std::mt19937 rng{std::random_device{}()};

    // Find all tiles of the given type and spread to neighbours
    std::vector<Vector2D> sources;
    for (int32_t y = 0; y < board_->height(); ++y) {
        for (int32_t x = 0; x < board_->width(); ++x) {
            if (board_->at({x, y}).type() == type) {
                sources.push_back({x, y});
            }
        }
    }

    const Vector2D dirs[] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    std::uniform_int_distribution<int> spreadChance(0, 100);

    for (const auto& src : sources) {
        for (const auto& dir : dirs) {
            Vector2D neighbour = src + dir;
            if (board_->inBounds(neighbour) &&
                board_->at(neighbour).type() == TileType::Normal &&
                spreadChance(rng) < 30) {
                board_->setTileType(neighbour, type);
            }
        }
    }
}

void BoardEvolution::meltIceTiles() {
    if (!board_) return;

    for (int32_t y = 0; y < board_->height(); ++y) {
        for (int32_t x = 0; x < board_->width(); ++x) {
            if (board_->at({x, y}).type() == TileType::Ice) {
                // 50% chance to melt each turn
                static std::mt19937 rng{std::random_device{}()};
                std::uniform_int_distribution<int> chance(0, 100);
                if (chance(rng) < 50) {
                    board_->setTileType({x, y}, TileType::Water);
                }
            }
        }
    }
}

void BoardEvolution::randomTileMutation() {
    if (!board_) return;

    static std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> xDist(0, board_->width() - 1);
    std::uniform_int_distribution<int> yDist(0, board_->height() - 1);
    std::uniform_int_distribution<int> typeDist(0, static_cast<int>(TileType::COUNT) - 1);

    Vector2D pos{xDist(rng), yDist(rng)};
    if (board_->inBounds(pos)) {
        TileType newType = static_cast<TileType>(typeDist(rng));
        board_->setTileType(pos, newType);
    }
}
