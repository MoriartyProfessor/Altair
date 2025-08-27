![](res/eagle.jpg)

Altair - UCI Compatible chess engine
=======================================

* * *

**Altair** is a strong (hopefully in the future) UCI compatible chess engine written in C++.

## Features list

Board representation:

- [x] Move generation (with Magic Bitboards)
- [x] Constrution of position from FEN

Search:
- [x] Negamax
- [ ] Alpha-Beta pruning
- [ ] Move Ordering (preferably with Killer Heuristics)
- [ ] Iterative Deepening
- [ ] Transposition tables (with Zobrist Hashing)
- [ ] Quiescence search
- [ ] Aspiration windows
- [ ] Principled Variation search
- [ ] Search Extensions Reductions
- [ ] Late Move Reduction (LMR)
- [ ] Null Move Pruning

Evaluation:
- [x] Material
- [ ] Game-Phase dependent evaluation
- [ ] Piece-Square Tables
- [ ] Pawn Structure
- [ ] Evaluation of Pieces
- [ ] Evaluation Patterns
- [ ] Mobility
- [ ] Center Control
- [ ] Connectivity
- [ ] Trapped Pieces
- [ ] King Safety
- [ ] Space
- [ ] Tempo

Other game phases:
- [ ] Opening book
- [ ] Endgame tablebases

Miscellaneous:
- [x] UCI protocol implementation
- [ ] Time control
- [ ] Parallel search

Tests:
- [x] Perft Test


## Dependencies

## Building
