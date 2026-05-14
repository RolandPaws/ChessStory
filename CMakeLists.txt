cmake_minimum_required(VERSION 3.20)
project(ChessRoguelike VERSION 0.1.0 LANGUAGES CXX)

# ── C++ Standard ──────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── Build Options ─────────────────────────────────────────────────────────────
option(BUILD_TESTS "Build unit tests" ON)
option(ENABLE_SANITIZERS "Enable address/undefined sanitizers" OFF)

# ── Global Include Paths ──────────────────────────────────────────────────────
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
)

# ── Source Tree ───────────────────────────────────────────────────────────────
add_subdirectory(src)

# ── Tests ─────────────────────────────────────────────────────────────────────
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ── Dependencies (placeholder for future package managers) ────────────────────
# find_package(fmt CONFIG QUIET)
# find_package(spdlog CONFIG QUIET)
# find_package(SDL2 CONFIG QUIET)
