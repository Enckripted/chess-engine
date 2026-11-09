cmake_minimum_required(VERSION 4.3.0)
project(chess-engine VERSION 0.1.0 LANGUAGES C CXX)


add_executable(chess-engine)
target_sources(chess-engine
PUBLIC
src/main.cpp
src/game/move.cpp
src/game/board.cpp
src/game/space.cpp
src/game/game_result.cpp
src/move_generator.cpp
src/tui.cpp
)

target_link_options(chess-engine PRIVATE -static)
set_property(TARGET chess-engine PROPERTY CXX_STANDARD 17)