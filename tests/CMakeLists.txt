# ── Test executable ───────────────────────────────────────────────────────────
add_executable(chessroguelike_tests
    test_board.cpp
    test_pieces.cpp
    test_movement.cpp
)

target_link_libraries(chessroguelike_tests PRIVATE
    chesscore
)

add_test(NAME ChessRoguelikeTests COMMAND chessroguelike_tests)
