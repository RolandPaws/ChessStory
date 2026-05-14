# ── Gather Sources ────────────────────────────────────────────────────────────
file(GLOB_RECURSE CHESS_ROGUELIKE_SOURCES
    "*.cpp"
)

# ── Library (core game logic, no main) ────────────────────────────────────────
add_library(chesscore
    ${CHESS_ROGUELIKE_SOURCES}
)

target_include_directories(chesscore PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
)

# ── Executable ────────────────────────────────────────────────────────────────
add_executable(chessroguelike
    main.cpp
)

target_link_libraries(chessroguelike PRIVATE
    chesscore
)
