#include <iostream>
#include "board.h"

/**
* Constructor. Just calls init()
*/
TBoard::TBoard()
{
  init();  
}

TBoard::~TBoard()
{
}

/**
* Initializes the board. Constructs the usual pattern in the middle of 
* the board and empties the rest of the squares.
*/
void TBoard::init()
{
	int i, j;

	for(i=0;i<BOARDSIZE;i++)
		for(j=0;j<BOARDSIZE;j++)
			board[i][j]=BLANK;

	board[3][3]=BLACK;
	board[4][4]=BLACK;
	board[3][4]=WHITE;
	board[4][3]=WHITE;
}

/**
* Returns the color of the stone in the given location.
* @return BLANK/BLACK/WHITE, the contents of the location. If
* the location is invalid, returns BLANK
* @param TCoord location, the co-ordinates of the square.
*/
unsigned int TBoard::stoneAt(TCoord location)
{
	/// First checks if given parameters are legal. 
	if(location.x < 0 || location.x >= BOARDSIZE ||
		location.y < 0 || location.y >= BOARDSIZE)
		return BLANK;

	/// Checks the contents of the square
	if(board[location.x][location.y] == WHITE)
		return WHITE;
	else
	{
		if(board[location.x][location.y] == BLACK)
			return BLACK;
		else
			return BLANK;
	}
}

/**
* Same as stoneAt but can be called without TCoord-type. Copies given
* values to TCoord and calls stoneAt();
* @return BLANK/BLACK/WHITE, depending on the contents of the location.
* @param unsigned int x, x-co-ordinate
* @param unsigned int y, y-co-ordinate
*/
unsigned int TBoard::getStoneAt(unsigned int x, unsigned int y)
{
  TCoord temp;
	temp.x=x;
	temp.y=y;

	return stoneAt(temp);
}


/**
* Inserts a stone in given location.
* @return If there are no problems, returns 0. If there's a stone
* or the given parameters are wrong, returns -1.
* @param TCoord location, the co-ordinates for the new stone
* @param unsigned int newStone, the color of the new stone.
*/
int TBoard::setStone(TCoord location, unsigned int newStone)
{
	if((newStone != BLACK) && (newStone != WHITE))
		return -1;

	if(stoneAt(location) == BLANK)
	{
		board[location.x][location.y] = newStone;
		return 0;
	}
	else
		return -1;
}

/**
* Turns the stone in the given co-ordination.
* @return 0, if there are no problems. -1, if there's no stone in the location
* or it is out of bounds.
* @param TCoord location, co-ordinates of the stone
*/
int TBoard::flipStone(TCoord location)
{
	if(board[location.x][location.y] == WHITE)
		board[location.x][location.y] = BLACK;
	else{
		if(board[location.x][location.y] == BLACK)
			board[location.x][location.y] = WHITE;
		else
			return -1;
	}
	return 0;
}

