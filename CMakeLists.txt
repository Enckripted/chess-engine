cmake_minimum_required(VERSION 4.3.0)
project(chess-engine VERSION 0.1.0 LANGUAGES C CXX)

add_executable(chess-engine)
target_sources(chess-engine
PUBLIC
src/main.cpp
src/game/move.cpp
src/game/board.cpp
)
# target_include_directories(chess-engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)