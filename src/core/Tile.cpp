#include "core/Tile.h"

Tile::Tile(TileType type, Vector2D position) noexcept
    : type_(type)
    , position_(position)
{
}
