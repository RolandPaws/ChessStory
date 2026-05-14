#pragma once

#include <cstdint>
#include <compare>
#include <functional>

/**
 * @brief Immutable 2D coordinate used for board positions and movement vectors.
 */
struct Vector2D {
    int32_t x = 0;
    int32_t y = 0;

    constexpr Vector2D() noexcept = default;
    constexpr Vector2D(int32_t x, int32_t y) noexcept : x(x), y(y) {}

    // ── Arithmetic ────────────────────────────────────────────────────────────
    constexpr Vector2D operator+(Vector2D rhs) const noexcept { return {x + rhs.x, y + rhs.y}; }
    constexpr Vector2D operator-(Vector2D rhs) const noexcept { return {x - rhs.x, y - rhs.y}; }
    constexpr Vector2D operator*(int32_t s) const noexcept { return {x * s, y * s}; }
    constexpr Vector2D operator-() const noexcept { return {-x, -y}; }

    constexpr Vector2D& operator+=(Vector2D rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }
    constexpr Vector2D& operator-=(Vector2D rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }

    // ── Comparison ────────────────────────────────────────────────────────────
    constexpr auto operator<=>(const Vector2D&) const noexcept = default;

    // ── Utility ───────────────────────────────────────────────────────────────
    [[nodiscard]] constexpr bool isZero()  const noexcept { return x == 0 && y == 0; }
    [[nodiscard]] constexpr int32_t manhattan() const noexcept { return std::abs(x) + std::abs(y); }
    [[nodiscard]] constexpr int32_t chebyshev() const noexcept { return std::max(std::abs(x), std::abs(y)); }

    // Direction helpers
    static constexpr Vector2D Up()    noexcept { return { 0, -1}; }
    static constexpr Vector2D Down()  noexcept { return { 0,  1}; }
    static constexpr Vector2D Left()  noexcept { return {-1,  0}; }
    static constexpr Vector2D Right() noexcept { return { 1,  0}; }
};

// Hash support for use in unordered containers
template<>
struct std::hash<Vector2D> {
    constexpr std::size_t operator()(const Vector2D& v) const noexcept {
        return std::hash<int32_t>{}(v.x) ^ (std::hash<int32_t>{}(v.y) << 1);
    }
};
