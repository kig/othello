#include <iostream>
#include "rules.h"
#include "board.h"

/**
* Constructor. Calls for the boards init.
*/
TRules::TRules() : TBoard(), weird(FALSE)
{
 init();
}

TRules::TRules(bool weird) : TBoard(), weird(weird)
{
 init();
}

TRules::~TRules()
{
}


/**
* First checks if location is legal and then puts a stone there.
* After that flips all trapped stones.
* @param TCoord location, the co-ordinates of the new stone.
* @param  unsigned int newStone, color of new stone
* @return Returns -1 if the place is illegal. Else returns 0.
*/
int TRules::rulesSetStone(TCoord location, unsigned int newStone)
{
	/// Checks that the location is empty.
	if(stoneAt(location) != BLANK)
		return -1;

	/// Checks that the location is legal and turns the stones.
	if(turnStones(location, newStone) == -1)
		return -1;

	setStone(location, newStone);

	return 0;
}

/**
* Flips trapped stones between given location and next own stone
* Calls flipAllDirections with flip=1.
* @param TCoord location, place of own stone.
* @param unsigned int newStone, own color.
* @return Returns 0, if move is lega. If no stones are turned, returns -1.
*/
int TRules::turnStones(TCoord location, unsigned int newStone)
{
  return flipAllDirections(location,newStone,1);
}

/**
* Checks if player has any possible moves left. Goes through the board until
* finds an empty space and then checks if there are trapped stones between it and
* next own stone.
* @param unsigned int next, color of next player.
* @return Returns 0, if found possible moves. -1, if not.
*/

int TRules::movePossible(unsigned int next)
{
  int i, j, k; 
  // int possibleMoves = 0;
  TCoord location;
  
  for (i=0; i<BOARDSIZE; i++) {
      for (j=0; j<BOARDSIZE; j++) {
           if (getStoneAt(i,j) == BLANK) {
             location.x = i;
             location.y = j;

			 /// Only checks, does not turn the stones
             k = flipAllDirections(location,next, 0);
             if (k == 0) {
              // possibleMoves++; // legal move
			  return 0;
             }
           }
      }

  }

/*  if (possibleMoves > 0) {
      return 0; // There are moves left.
  } else {
      return -1; // No moves.
  }
*/

  return -1;
}

/**
* Checks the board and counts the number of white stones
* @return Returns score.
*/

int TRules::scoreWhite()
{
	int i, j, score=0;

	for(i=0; i<BOARDSIZE; i++)
		for(j=0; j<BOARDSIZE; j++)
		{
			if(getStoneAt(i,j) == WHITE)
				score++;
		}

	return score;
}

/**
* Counts the number of black stones
* @return Returns score.
*/
int TRules::scoreBlack()
{
	int i, j, score=0;

	for(i=0; i<BOARDSIZE; i++)
		for(j=0; j<BOARDSIZE; j++)
		{
			if(getStoneAt(i,j) == BLACK)
				score++;
		}

	return score;
}


/**
* Checks each direction around the stone put in given location for turnable stones.
* Also turns the stones if needed.
* @param TCoord location, co-ordinates for the new stone.
* @param unsigned int newStone, color of the new stone
* @param int flip, if flip==1, turns the stones. Else do nothing with them.
* @return Returns 0 if move is legal, else -1.
*/
int TRules::flipAllDirections(TCoord location, unsigned int newStone, int flip) {
  int i, retval=0;
  for (i=0; i<8; i++) {
     retval += flipDir(location,i,newStone,flip);
  }
  retval = (retval > -8) ? 0 : -1;
  return retval;
}

/**
* Checks one direction from the given location for trapped stones. Turns
* the stones if necessary.
* @param TCoord location, location of the new stone
* @param int dir, direction which to check
* @param unsigned int newStone, color of the new stone
* @param int flip, do we turn the stones or not. If flip==1, then yes.
* @return Returns 0, if trapped stones found, else -1.
*/
int TRules::flipDir(TCoord location,int dir, unsigned int newStone, int flip)
{
  TCoord next = getNextLocation(location,dir);
  unsigned int nextStone = stoneAt(next);

  if (!weird) {

    /// Follow own stones in given direction and check square at the end of own stones.
    while (nextStone != newStone && nextStone != BLANK) {
      next = getNextLocation(next,dir);
      nextStone = stoneAt(next);
      }
    if (nextStone == BLANK)
      return -1;

  }
            
  next = getNextLocation(location,dir);
  nextStone = stoneAt(next);
        
  if (nextStone != newStone && nextStone != BLANK) {
    /// Flips the stones
    flipDir(next, dir, newStone, flip);
    if (flip == 1) flipStone(next);
    return 0;
  } else {
    return -1;
  }
}

/**
* Returns the co-ordinates for the next square in given direction.
* @param TCoord location, present location
* @param int dir, direction. 0-up, 1-up and right, 2-right, 3-down and right, 4-down, 5-left and down, 6-left, 7-left and up
* @return TCoord, location of the next stone
*/

TCoord TRules::getNextLocation(TCoord location, int dir)
{
  TCoord retval;
  int x=location.x, y=location.y;
  switch (dir)
  {
  case 0 : // up
    y++;
    break;
  case 1 : // up and right
    x++; y++;
    break;
  case 2 : // right
    x++;
    break;
  case 3 : // down and right
    x++; y--;
    break;
  case 4 : // down
    y--;
    break;
  case 5 : // left and down
    x--; y--;
    break;
  case 6 : // left
    x--;
    break;
  case 7 : // left and up
    x--; y++;
    break;
  }
  retval.x = x;
  retval.y = y;
  
  return retval;
}





