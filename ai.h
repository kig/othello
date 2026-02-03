/** @file ai.h
* Contains the AI. Basic idea is to first search all possible moves
* for the player and then by comparing them by various criterias finding
* the best place and returning it.
* @author Jari Väisänen
*/

#ifndef AI_H
#define AI_H

#include <iostream>
#include <stdlib.h>
#include <limits.h>
#include "rules.h"

#define BLOCKSIZE 10
/// Number of steps to look forward
#define STEPS 2 

/**
* Finds all moves for player and returns the co-ordinates for the next move.
*
* @return TCoord, location of the best next move.
* @param TRules othello, current situation
* @param unsigned int player, color.
*/
TCoord findMove(TRules othello, unsigned int player);

/**
* Examines next own move and searches all opponent's moves.
* @return Returns the best score for this move.
* @param TRules othello, current situation.
* @param unsigned int player, players color
* @param TCoord place, place to try
* @param int steps, amount of steps left
*/
int tryNext(TRules othello, unsigned int player, TCoord place, int steps);
/**
* Make the next opponent's move and search all possible next own moves.
* @param TRules othello, current situation.
* @param unsigned int player, player's color.
* @param TCoord place, enemy's move.
* @param int steps, remaining steps
* @return Returns the score for this move.
*
*/
int nextEnemyMove(TRules othello, unsigned int player, TCoord place, int steps);


/**
* Evaluates the value of given move. Counts the amount of liberties,
* amount of turned stones, value of the location, etc.
* @param TRules othello, current situation.
* @param unsigned int player, current player.
* @param TCoord place, location.
* @return Returns the value of the place.
*/
int moveScore(TRules othello, unsigned int player, TCoord place);


/**
* Counts the amount of turned stones if stone is put in given
* location.
* @param TRules othello, current situation.
* @param unsigned int player, player's color.
* @param TCoord place, location for the stone.
* @return Returns the number of turned stones.
*/
int turnedStones(TRules othello, unsigned int player, TCoord place);

/**
* Counts the number of empty spaces connected to own squares.
* @param TRules othello, current situation.
* @param unsigned int player, player's color.
*/
int countLiberties(TRules othello, unsigned int player);

/**
* Checks the location and gives points based on it's location. Corners
* are best, edge is good, etc.
* @param TRules othello, current board.
* @param TCoord place, co-ordinates of the move
* @param unsigned int player, player's color.
* @return Returns the value of location
*/
int evaluateLocation(TRules othello, TCoord place, unsigned int player);

/**
* Searches for all possible moves for player.
* @param TRules othello, current board.
* @param unsigned int player, player's color.
* @param int *moveNumber, saves the number of moves here.
* @return Returns the list of possible moves.
*/
TCoord *possibleMoves(TRules othello, unsigned int player, int *moveNumber);

/**
* Returns the vector for direction k.
* @param int k, 1-right, 2-up and right, 3-up, 4-up and left, 5-left 6-down and left, 7-down, 8-down and right.
* @return Returns the vector.
*/
TCoord getDirection(int k);

/**
* Sorts the scores based on their scores.
* @param TRules othello, current situation.
* @param TCoord *moves, list of moves.
* @param int moveNum, number of moves.
* @param unsigned int player, player's color.
*/
void sortScores(TRules othello, TCoord *moves, int moveNum, unsigned int player);

#endif /*AI_H*/
