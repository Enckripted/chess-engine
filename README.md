# chess-engine

The goal is that at some point this will become a ches engine that works with UCI, currently it is nowhere near close.
This is currently just a not fully implemented chess player, where every piece can move but rules like check and castling aren't implemented yet.

Also this is my first time making a C++ project, so things are very messy. Next week I'm going to tidy up everything and get the build system properly working, and hopefull by then there'll at least be a computer opponent.

To play, run the executable, and send your moves in UCI notation (so start square and the end square, 1. e4 becomes e2e4 and so on with no need to mark captures). You win by taking the opponent's king.
