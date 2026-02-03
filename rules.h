/** @file rules.h
 *  Contains the rules for othello
 *  @author Ilmari Heikkinen, Jari Väisänen
*/

#ifndef RULES_H
#define RULES_H

#define TRUE 1
#define FALSE 0

#include "board.h"

/**
* @short Rules for othello.
* 
* Contains the board and rules for othello. Has the needed methods
* for inserting a stone, flipping stones and getting score.
*/

class TRules : public TBoard
{

public:

/**
* Constructor. Calls for the boards init.
*/
TRules();
TRules(bool weird);
~TRules();

/**
* First checks if location is legal and then puts a stone there.
* After that flips all trapped stones.
* @param TCoord location, the co-ordinates of the new stone.
* @param  unsigned int newStone, color of new stone
* @return Returns -1 if the place is illegal. Else returns 0.
*/
int rulesSetStone(TCoord location, unsigned int newStone);

/**
* Flips trapped stones between given location and next own stone
* Calls flipAllDirections with flip=1.
* @param TCoord location, place of own stone.
* @param unsigned int newStone, own color.
* @return Returns 0, if move is lega. If no stones are turned, returns -1.
*/
int turnStones(TCoord location, unsigned int newStone);

/**
* Checks if player has any possible moves left. Goes through the board until
* finds an empty space and then checks if there are trapped stones between it and
* next own stone.
* @param unsigned int next, color of next player.
* @return Returns 0, if found possible moves. -1, if not.
*/
int movePossible(unsigned int next);


/**
* Checks the board and counts the number of white stones
* @return Returns score.
*/
int scoreWhite();

/**
* Counts the number of black stones
* @return Returns score.
*/
int scoreBlack();

private:

/**
* Checks each direction around the stone put in given location for turnable stones.
* Also turns the stones if needed.
* @param TCoord location, co-ordinates for the new stone.
* @param unsigned int newStone, color of the new stone
* @param int flip, if flip==1, turns the stones. Else do nothing with them.
* @return Returns 0 if move is legal, else -1.
*/
int flipAllDirections(TCoord location, unsigned int newStone, int flip = 1);

/**
* Checks one direction from the given location for trapped stones. Turns
* the stones if necessary.
* @param TCoord location, location of the new stone
* @param int dir, direction which to check
* @param unsigned int newStone, color of the new stone
* @param int flip, do we turn the stones or not. If flip==1, then yes.
* @return Returns 0, if trapped stones found, else -1.
*/
int flipDir(TCoord location,int dir, unsigned int newStone, int flip = 1);

/**
* Returns the co-ordinates for the next square in given direction.
* @param TCoord location, present location
* @param int dir, direction. 0-up, 1-up and right, 2-right, 3-down and right, 4-down, 5-left and down, 6-left, 7-left and up
* @return TCoord, location of the next stone
*/
TCoord getNextLocation(TCoord location, int dir);

/**
* Play othello with weird rules? [y/N]
*/
bool weird;

};

#endif /* RULES_H */
