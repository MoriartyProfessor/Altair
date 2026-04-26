![](res/eagle.jpg)

Gwaihir - UCI Compatible chess engine
=======================================

* * *

**Gwaihir** is a strong (hopefully in the future) UCI compatible chess engine written in C++.

## Features list

Board representation:

- [x] Move generation (with Magic Bitboards)
- [x] Constrution of position from FEN

Search:
- [x] Negamax
- [x] Alpha-Beta pruning
- [x] Move Ordering (Hash > MVV-LVA > Killer > History)
- [x] Iterative Deepening
- [x] Transposition tables (with Zobrist Hashing)
- [x] Quiescence search
- [x] Principled Variation search
- [ ] Aspiration windows
- [ ] Principled Variation search
- [ ] Search Extensions Reductions
- [ ] Late Move Reduction (LMR)
- [ ] Null Move Pruning
- [ ] Lazy SMP

Evaluation:
- [x] Material
- [x] Game-Phase dependent evaluation
- [x] Piece-Square Tables
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
- [ ] Evaluation/Pawn TT

Other game phases:
- [ ] Opening book
- [ ] Endgame tablebases

Miscellaneous:
- [x] UCI protocol implementation
- [x] Time control

Tests:
- [x] Perft Test


## Current To-Do list
* Experiment with TT sizes
* Write python script to extract results of the tournament from PGN
* Figure out why are so few nodes searched if compiled with RelWithDebInfo
* 50 move rule
* MNPS drops significantly towards the endgame. Potential causes:
    * More TT hits, so nodes are not being counted.
    * History stack is getting too big.
* Minor - don't make illegal move when getting mated
* Minor - stop iterative deepening when there is a mate already

## Improvements

* Extensions and Reductions
* NMP

## Dependencies

## Building
