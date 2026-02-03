/** @file board.h
 *  Contains the information for othelloboard.
 *  @author Ilmari Heikkinen, Jari Väisänen
*/

#ifndef BOARD_H
#define BOARD_H

#include <iostream>

#define BOARDSIZE 8
#define ROWS 8
#define COLUMNS 8

/// Contents of a square
#define BLANK 0
#define BLACK 1
#define WHITE 2


/**
* @short Type for co-ordinates
*
* Used to express the place of squares in the board.
* 0,0 is the upper-left-corner and 7,7 is the lower-right-corner.
* @param int x, int y. x- and y-co-ordinates
*/
typedef struct{
	int x;
	int y;
	} TCoord;

/**
* @short Contains the board and the methods for manipulating it
* 
* The board is an 8x8 array. Class contains the methods for inserting a 
* piece, turning a stone and returning the contents of a square.
*/
class TBoard
{
	public:

/**
* Constructor. Just calls init()
*/
TBoard();

~TBoard();

/**
* Initializes the board. Constructs the usual pattern in the middle of 
* the board and empties the rest of the squares.
*/
void init();

/**
* Returns the color of the stone in the given location.
* @return BLANK/BLACK/WHITE, the contents of the location. If
* the location is invalid, returns BLANK
* @param TCoord location, the co-ordinates of the square.
*/
unsigned int stoneAt(TCoord location);

/**
* Same as stoneAt but can be called without TCoord-type. Copies given
* values to TCoord and calls stoneAt();
* @return BLANK/BLACK/WHITE, depending on the contents of the location.
* @param unsigned int x, x-co-ordinate
* @param unsigned int y, y-co-ordinate
*/
unsigned int getStoneAt(unsigned int x, unsigned int y);

/**
* Inserts a stone in given location.
* @return If there are no problems, returns 0. If there's a stone
* or the given parameters are wrong, returns -1.
* @param TCoord location, the co-ordinates for the new stone
* @param unsigned int newStone, the color of the new stone.
*/
int setStone(TCoord location, unsigned int newStone);

/**
* Turns the stone in the given co-ordination.
* @return 0, if there are no problems. -1, if there's no stone in the location
* or it is out of bounds.
* @param TCoord location, co-ordinates of the stone
*/
int flipStone(TCoord location);


  protected:

/**
* Board. Values of the array are BLANK/BLACK/WHITE depending on the
* color of the stone on the square.
*/
unsigned int board[BOARDSIZE][BOARDSIZE];


};

#endif /* BOARD_H */
