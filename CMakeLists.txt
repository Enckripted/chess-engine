cmake_minimum_required(VERSION 4.3.0)
project(chess-engine VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)
FetchContent_Declare(
	argparse
	GIT_REPOSITORY https://github.com/p-ranav/argparse.git
	GIT_TAG v3.2
)
FetchContent_MakeAvailable(argparse)

add_executable(chess-engine)
target_sources(chess-engine
PUBLIC
src/main.cpp
src/game/move.cpp
src/game/board.cpp
src/game/space.cpp
src/game/game_result.cpp
src/move_generator.cpp
src/perft.cpp
src/tui.cpp
)

target_link_libraries(chess-engine argparse)
target_link_options(chess-engine PRIVATE -static)
set_property(TARGET chess-engine PROPERTY CXX_STANDARD 20)