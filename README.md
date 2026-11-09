# chess-engine

The goal is to make this a proper chess engine that has UCI support, and along the process have me learn the basics of C++. Currently, it only almost simulates the full game of chess with the exception of:

- Castling (I ran out of time)
- All the million ways you can stalemate

I had planned to implement a proper [perft](https://chessprogramming.org/Perft) to make sure all my move generation code is actually properly working but I ran out of time this week.

# Usage

You should just be able to run the executable or call it in the command line as "chess-engine."

Being able to input moves through algebraic notation is something I plan on adding but once again, I ran out of time. You have to instead use [UCI notation](https://en.wikipedia.org/wiki/Universal_Chess_Interface), where every move follows the format start square, end square, and promotion (if applicable) concatenated together without spaces.

If you for some reason want to compile from source, CMake uses G++ from the MSYS2 collection, which I installed using [this tutorial](https://code.visualstudio.com/docs/cpp/config-mingw). Then just build using CMake.
