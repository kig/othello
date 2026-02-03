/***************************************************************************
                          gamerules.cpp  -  description
                             -------------------
    begin                : Sat May 3 2003
    copyright            : (C) 2003 by Ilmari Heikkinen
    email                : irheikki@cc.hut.fi
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qvaluelist.h>
#include <memory>

#include "gamerules.h"
#include "rules.h"
#include "board.h"


GameRules::GameRules(bool weird) : TRules(weird),
                         board(new QValueList<unsigned int>),
                         changes(new QValueList<unsigned int>)
{
  for (int i=0; i<ROWS*COLUMNS; i++) {
    board->append(getPiece(i));
  }
  turn = BLACK;
}

GameRules::~GameRules() {}

// set piece at pos, return false if illegal
bool GameRules::setPiece(unsigned int pos, unsigned int color)
{
  if (turn != BLANK) {
    if (color == BLANK) {color = turn;}
    if (rulesSetStone(uiToTCoord(pos), color) == 0) {
      updateBoard(pos);
      updateTurn();
      return true;
    } else {
      return false;
    }

  } else {
    return false;
  }
}


// get piece at pos
unsigned int GameRules::getPiece(unsigned int pos)
{
  return stoneAt(uiToTCoord(pos));
}


// get active player, BLANK is game over
unsigned int GameRules::getTurn()
{
  return turn;
}


// get player score, BLACK/WHITE, returns amount of pieces
unsigned int GameRules::getScore(unsigned int color)
{
  if (color == BLACK) {
    return scoreBlack();
  } else {
    return scoreWhite();
  }
}


// get changed positions, first item in list is the new piece color followed by position
// uses BLACK BLANK WHITE
QValueList<unsigned int>::iterator GameRules::getChanges()
{
  QValueList<unsigned int>::iterator it = changes->begin();
  return it;
}


// get current board status.
QValueList<unsigned int>::iterator GameRules::getBoard()
{
  QValueList<unsigned int>::iterator it = board->begin();
  return it;
}


// convert unsigned int to TCoord
TCoord GameRules::uiToTCoord(unsigned int pos)
{
  TCoord retval;
  retval.x=pos%8;
  retval.y=pos/8;
  return retval;
}

// convert TCoord to unsigned int
unsigned int GameRules::TCoordToUi(TCoord pos)
{
  unsigned int retval;
  retval =pos.x;
  retval+=pos.y*8;
  return retval;
}



// PRIVATE

// updates Board, updates Changes 
void GameRules::updateBoard(unsigned int pos)
{
  int i;
  
  std::auto_ptr< QValueList<unsigned int> > temp( new QValueList<unsigned int> );
  
  QValueList<unsigned int>::iterator itt;
  QValueList<unsigned int>::iterator itb;

  for ( itb = board->begin(); itb != board->end(); ++itb ) {
    temp->append((*itb));
  }
  
  // clear & update board
  board->clear();
  for (i=0; i<ROWS*COLUMNS; i++) {
    board->append(getPiece(i));
  }

  // clear & update changes to board
  changes->clear();
  changes->append(getTurn());
  changes->append(pos);
  
  // i is current position
  for ( itt = temp->begin(), itb = board->begin(), i=0;
        itt != temp->end();
        ++itt, ++itb, i++
      ) {
    if ( (*itt)!=(*itb) ) {
      changes->append(i);
    }
  }
}



// updates turn, sets to BLANK if game over
void GameRules::updateTurn()
{
      unsigned int newTurn = (getTurn() == BLACK) ? WHITE : BLACK; // opposite of turn

      if ( movePossible(newTurn)==0 ) { // other player can move
        turn = newTurn;
      } else if ( movePossible(turn)==-1 ) { // neither player can move
        turn = BLANK;
      }
}
