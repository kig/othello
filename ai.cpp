#include <iostream>
#include <stdlib.h>
#include "ai.h"
#include "rules.h"

/**
* Finds all moves for player and returns the co-ordinates for the next move.
*
* @return TCoord, location of the best next move.
* @param TRules othello, current situation
* @param unsigned int player, color.
*/
TCoord findMove(TRules othello, unsigned int player)
{
	TCoord *moveList;
	TCoord nextMove;
	int moveNumber=0;
	int k, previousScore, newScore;

        /// seed rand() with processor ticks
        srand( (int)clock() );

	/// Find all moves and sort them.
	moveList=possibleMoves(othello, player, &moveNumber);
	sortScores(othello, moveList, moveNumber, player);

	nextMove=moveList[0];
	previousScore=tryNext(othello, player, nextMove, STEPS);
	
	/// Examine third of the best looking moves deeper.
	for(k=1;k<(int)moveNumber/3;k++)
	{
		if((newScore=tryNext(othello, player, moveList[k], STEPS)) > previousScore)
		{
			nextMove=moveList[k];
			previousScore=newScore;
		} else if(newScore == previousScore && ((float)rand()/(float)0x7fffffff)>0.5)
		{
			nextMove=moveList[k];
			previousScore=newScore;
		}
	}
	
	free(moveList);

  return nextMove;
}

/**
* Examines next own move and searches all opponent's moves.
* @return Returns the best score for this move.
* @param TRules othello, current situation.
* @param unsigned int player, players color
* @param TCoord place, place to try
* @param int steps, amount of steps left
*/
int tryNext(TRules othello, unsigned int player, TCoord place, int steps)
{
	int score, tempScore;
	unsigned int enemy;
	TCoord *enemyMoves;
	int moveNumber;
	int i;

	if(player==WHITE)
		enemy=BLACK;
	else
		enemy=WHITE;

	score=moveScore(othello, player, place);
	/// Places the stone on copy of the board.
	othello.rulesSetStone(place, player);

/// If we have come to the end of the path, return the score
/// for this move. Else study next moves for the enemy and count the
/// score.
	if(steps>0)
	{
		enemyMoves=possibleMoves(othello, enemy, &moveNumber);
		sortScores(othello, enemyMoves, moveNumber, enemy);

		/// Check part of the enemy's moves.
		for(i=0;i<(int) moveNumber/5;i++)
		{
			tempScore=nextEnemyMove(othello, player, enemyMoves[i], steps-1);
			if(tempScore>score)
				score=tempScore;
		}
		
		free(enemyMoves);
	}

	return score;
}

/**
* Make the next opponent's move and search all possible next own moves.
* @param TRules othello, current situation.
* @param unsigned int player, player's color.
* @param TCoord place, enemy's move.
* @param int steps, remaining steps
* @return Returns the score for this move.
*
*/
int nextEnemyMove(TRules othello, unsigned int player, TCoord place, int steps)
{
	unsigned int enemy;
	TCoord *moveList;
	int moveNumber;
	int score=0;
	int tempScore;
	int i;

	if(player==WHITE)
		enemy=BLACK;
	else
		enemy=WHITE;

	/// Place opponent's stone
	othello.rulesSetStone(place, enemy);

	/// Get next possible moves and sort them.
	moveList=possibleMoves(othello, player, &moveNumber);
	sortScores(othello, moveList, moveNumber, player);

	/// Check part of the best moves and return the score for the best.
	for(i=0;(int) i<moveNumber/4;i++)
	{
		if(i==0)
			score=tryNext(othello, player, moveList[0], steps);
		else
		{
			tempScore=tryNext(othello, player, moveList[i], steps);
			if (tempScore>score)
				score=tempScore;
		}
	}

	free(moveList);

	return score;
}

/**
* Evaluates the value of given move. Counts the amount of liberties,
* amount of turned stones, value of the location, etc.
* @param TRules othello, current situation.
* @param unsigned int player, current player.
* @param TCoord place, location.
* @return Returns the value of the place.
*/
int moveScore(TRules othello, unsigned int player, TCoord place)
{
	int score=0;
	int newMoves;
	unsigned int enemy;
	TCoord *temp;

	if(player==WHITE)
		enemy=BLACK;
	else
		enemy=WHITE;

	//Checks the number of turned opponent's stones. Not as important as others.
	//score+=turnedStones(othello, player, place);

	score+=evaluateLocation(othello, place, player);

	othello.rulesSetStone(place, player);

	/// We're trying to avoid empty spaces around our stones
	score-=countLiberties(othello, player); 

	temp=possibleMoves(othello, player, &newMoves);
	free(temp);

	/// and we're trying to get maximum number of moves
	score+=newMoves;

	temp=possibleMoves(othello, enemy, &newMoves);
	
	/// and we don't want opponent to have many moves.
	score-=newMoves; 

	free(temp);

	return score;
}

/**
* Counts the amount of turned stones if stone is put in given
* location.
* @param TRules othello, current situation.
* @param unsigned int player, player's color.
* @param TCoord place, location for the stone.
* @return Returns the number of turned stones.
*/

int turnedStones(TRules othello, unsigned int player, TCoord place)
{
	int score=0;
	int enemyCount;
	TCoord direction, check;
	int k;
	unsigned int enemy;

	if(player==WHITE)
		enemy=BLACK;
	else
		enemy=WHITE;

	/// Check every direction
	for(k=1;k<=8;k++) 
	{
		direction=getDirection(k);

		check.x=place.x+direction.x;
		check.y=place.y+direction.y;

		if(othello.stoneAt(check) == enemy)
		{
			enemyCount=0;
			do
			{	
				enemyCount++;
				check.x+=direction.x;
				check.y+=direction.y;
			}
			while(othello.stoneAt(check) == enemy);
		      
			if(othello.stoneAt(check) == BLANK)
			score+=enemyCount;
		}
	}

	return score;
}

/**
* Counts the number of empty spaces connected to own squares.
* @param TRules othello, current situation.
* @param unsigned int player, player's color.
*/

int countLiberties(TRules othello, unsigned int player)
{
	int i, j,k;
	int liberties=0;
	int foundOwn;
	TCoord direction;

	/// Trying to find blank squares.
	for(i=0;i<BOARDSIZE;i++)
		for(j=0;j<BOARDSIZE;j++)
			if(othello.getStoneAt(i,j) == BLANK)
			{
				/// We've found a blank. Next check if there are own stones around it.
				foundOwn=0;

				/// Checks every direction.
				for(k=1;k<=8 && foundOwn==0;k++)
				{
					direction=getDirection(k);

					if(othello.getStoneAt(i+direction.x, j+direction.y) == player)
						foundOwn=1;
				}

				if(foundOwn==1)
					liberties++;
			}

	return liberties;
}


/**
* Checks the location and gives points based on it's location. Corners
* are best, edge is good, etc.
* @param TRules othello, current board.
* @param TCoord place, co-ordinates of the move
* @param unsigned int player, player's color.
* @return Returns the value of location
*/
int evaluateLocation(TRules othello, TCoord place, unsigned int player)
{
	unsigned int enemy;
	int i;

	if(player==WHITE)
		enemy=BLACK;
	else
		enemy=WHITE;
/*
	/// First check if it's a cornerplace. Corners are good. Very good. Yes.
	if((place.x==0 && place.y==0) ||
		(place.x==0 && place.y==BOARDSIZE-1) ||
		(place.x==BOARDSIZE-1 && place.y==0) ||
		(place.x==BOARDSIZE-1 && place.y==BOARDSIZE-1))
		return 40;
*/

	/// Checks if place is on edge. Then checks if it's connected to a corner
	/// with either own or opponent's stones. If own, good, if opponent's, bad.
	/// Else it's ok place. Probably.

	if(place.y==0)
	{
		for(i=0; i<place.x && othello.getStoneAt(i,0) == player;i++);
		if(i==place.x)
			return 40;
		else
		{
			for(i=BOARDSIZE-1; i>place.x && othello.getStoneAt(i,0) == player;i--);
			if(i==place.x)
				return 40;
		}
		for(i=0; i<place.x && othello.getStoneAt(i,0) == enemy;i++);
		if(i==place.x && othello.getStoneAt(i-1,0) == enemy)
			return -20;
		else
		{
			for(i=BOARDSIZE-1; i>place.x && othello.getStoneAt(i,0) == enemy;i--);
			if(i==place.x && othello.getStoneAt(i+1, 0) == enemy)
				return -20;
		}

		return 20;
	}

	if(place.y==BOARDSIZE-1)
	{
		for(i=0; i<place.x && othello.getStoneAt(i,BOARDSIZE-1) == player;i++);
		if(i==place.x)
			return 40;
		else
		{
			for(i=BOARDSIZE-1; i>place.x && othello.getStoneAt(i,BOARDSIZE-1) == player;i--);
			if(i==place.x)
				return 40;
		}
		for(i=0; i<place.x && othello.getStoneAt(i,BOARDSIZE-1) == enemy;i++);
		if(i==place.x && othello.getStoneAt(i-1, BOARDSIZE-1) == enemy)
			return -20;
		else
		{
			for(i=BOARDSIZE; i>place.x && othello.getStoneAt(i,BOARDSIZE-1) == enemy;i--);
			if(i==place.x && othello.getStoneAt(i+1, BOARDSIZE-1) == enemy)
				return -20;
		}

		return 20;
	}

	if(place.x==0)
	{
		for(i=0; i<place.y && othello.getStoneAt(0,i) == player;i++);
		if(i==place.y)
			return 40;
		else
		{
			for(i=BOARDSIZE-1; i>place.y && othello.getStoneAt(0,i) == player;i--);
			if(i==place.y)
				return 40;
		}
		for(i=0; i<place.y && othello.getStoneAt(0,i) == enemy;i++);
		if(i==place.y && othello.getStoneAt(0,i-1) == enemy)
			return -20;
		else
		{
			for(i=BOARDSIZE-1; i>place.y && othello.getStoneAt(0,i) == enemy;i--);
			if(i==place.y && othello.getStoneAt(0,i+1) == enemy)
				return -20;
		}

		return 20;
	}

	if(place.x==BOARDSIZE-1)
	{
		for(i=0; i<place.y && othello.getStoneAt(BOARDSIZE-1, i) == player;i++);
		if(i==place.y)
			return 40;
		else
		{
			for(i=BOARDSIZE-1; i>place.y && othello.getStoneAt(BOARDSIZE-1,i) == player;i--);
			if(i==place.y)
				return 40;
		}
		for(i=0; i<place.y && othello.getStoneAt(BOARDSIZE-1, i) == enemy;i++);
		if(i==place.y && othello.getStoneAt(BOARDSIZE-1, i-1) == enemy)
			return -20;
		else
		{
			for(i=BOARDSIZE-1; i>place.y && othello.getStoneAt(BOARDSIZE-1,i) == enemy;i--);
			if(i==place.y && othello.getStoneAt(BOARDSIZE-1,i+1) == enemy)
				return -20;
		}

		return 20;
	}	

/*	///Next check if the square is next to corner. If there's a own stone
	///in the corner, this is good. Else, it's bad.

	if((place.x==1 && place.y==0) || (place.x==0 && place.y==1))
	{
		if(othello.getStoneAt(0,0) == player)
			return 20;
		else
			return -10;
	}
	if((place.x==1 && place.y==BOARDSIZE-1) || (place.x==0 && place.y==BOARDSIZE-2))
	{
		if(othello.getStoneAt(0,BOARDSIZE-1) == player)
			return 20;
		else
			return -10;
	}
	if((place.x==BOARDSIZE-2 && place.y==0) || (place.x==BOARDSIZE-1 && place.y==1))
	{
		if(othello.getStoneAt(BOARDSIZE-1, 0))
			return 20;
		else
			return -10;
	}
	if((place.x==BOARDSIZE-2 && place.y==BOARDSIZE-1) || (place.x==BOARDSIZE-1 && place.y==BOARDSIZE-2))
	{
		if(othello.getStoneAt(BOARDSIZE-1, BOARDSIZE-1))
			return 20;
		else
			return -10;
	}
	
*/

	return 0;
}


/**
* Searches for all possible moves for player.
* @param TRules othello, current board.
* @param unsigned int player, player's color.
* @param int *moveNumber, saves the number of moves here.
* @return Returns the list of possible moves.
*/
TCoord *possibleMoves(TRules othello, unsigned int player, int *moveNumber)
{
	TCoord *moveList;
	int blockNum=1;
	int x,y,k;
	unsigned int enemy;
	TCoord direction, check;

	*moveNumber=0;
	/// Allocates memory for the list.
	moveList=(TCoord *)malloc(BLOCKSIZE*sizeof(TCoord));
  
	if(player==WHITE)
		enemy=BLACK;
	else
		enemy=WHITE;
  
	/// Search every square for own stones.
	for(x=0; x<BOARDSIZE; x++)
	{
		for(y=0; y<BOARDSIZE; y++)
		{
			if(othello.getStoneAt(x,y) == player)
			{
				/// And check every direction for opponent's stones
				for(k=1;k<=8;k++)
				{
					direction=getDirection(k);
		  
					check.x=x+direction.x;
					check.y=y+direction.y;

					if(othello.stoneAt(check) == enemy)
					{
						/// Follow the trail until there are no more opponent's stones
						do
						{	
						check.x+=direction.x;
						check.y+=direction.y;
						} while(othello.stoneAt(check) == enemy);
		      
						/// If the next square was inside the board and it was empty, we have found a possible move. Add it to list and rejoice.
						if(othello.stoneAt(check) == BLANK && check.x>=0 && check.x<BOARDSIZE && check.y>=0 && check.y<BOARDSIZE)
						{
							(*moveNumber)++;

							/// We can allocate more memory for the list, if it's necessary.
							if(*moveNumber==blockNum*BLOCKSIZE)
							{
								blockNum++;
								moveList=(TCoord *) realloc(moveList, blockNum*BLOCKSIZE*sizeof(TCoord));
							}
							moveList[(*moveNumber)-1]=check;
						}
					}
				}
			}
		}
	}

  return moveList;
  
}

/**
* Returns the vector for direction k.
* @param int k, 1-right, 2-up and right, 3-up, 4-up and left, 5-left 6-down and left, 7-down, 8-down and right.
* @return Returns the vector.
*/
TCoord getDirection(int k)
{
	TCoord direction;

	switch (k)
	{
		case 1 : // Right
			direction.x=1;
			direction.y=0;
			break;
		case 2 : // Up and right
			direction.x=1;
			direction.y=1;
			break;
		case 3 : // Up
			direction.x=0;
			direction.y=1;
			break;
		case 4 : // Up and left
			direction.x=-1;
			direction.y=1;
			break;
		case 5 : // Left
			direction.x=-1;
			direction.y=0;
			break;	
		case 6 : // Down and left
			direction.x=-1;
			direction.y=-1;
			break;		
		case 7 : // Down
			direction.x=0;
			direction.y=-1;
			break;
		case 8 : // Down and right
			direction.x=1;
			direction.y=-1;
			break;
	}

	return direction;
}

/**
* Sorts the scores based on their scores.
* @param TRules othello, current situation.
* @param TCoord *moves, list of moves.
* @param int moveNum, number of moves.
* @param unsigned int player, player's color.
*/
void sortScores(TRules othello, TCoord *moves, int moveNum, unsigned int player)
{
	int i, j;
	TCoord temp;

	for(i=1;i<moveNum;i++)
	{
		temp.x=moves[i].x;
		temp.y=moves[i].y;
		j=i;

		while(j>0 && moveScore(othello, player,moves[j-1]) < moveScore(othello, player, temp))
		{
			moves[j].x=moves[j-1].x;
			moves[j].y=moves[j-1].y;
			j--;
		}

		moves[j].x=temp.x;
		moves[j].y=temp.y;
	}

}
