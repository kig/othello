/***************************************************************************
               gamerules.h  -  TRules GUI compatibility layer
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

#ifndef GAMERULES_H
#define GAMERULES_H

#include <memory>
#include <qvaluelist.h>

#include "rules.h"
#include "board.h"

using namespace std;

/**
* @short TRules wrapped into a GUI compatibility layer.
* @author Ilmari Heikkinen
*
* The GameRules class extends the TRules class and acts as a translation
* layer between the BoardView Othello GUI format calls and TRules internal format. It also provides a few
* convenience methods to ease GUI-TRules communication.
*
*/
class GameRules : public TRules
{
  public:
    /**
    * GameRules constructor. Calls the TRules constructor with the gametype.
    * @param weird TRUE for modified rules, FALSE for normal rules.
    */
    GameRules(bool weird=FALSE); // othello rules (true/false)
    /**
    * GameRules destructor.
    */
    ~GameRules();
    
    /**
    * Sets piece of color color to GUI-format (0..63) position pos on board.
    * @return Returns TRUE on success, FALSE on illegal move.
    * @param pos GUI-format (0..63) board position where to place the piece.
    * @param color Color of the piece to set, default is automatic based on current turn.
    */
    // set piece at pos, return false if illegal
    bool setPiece(unsigned int pos, unsigned int color = BLANK);

    /**
    * Gets color of piece on board in GUI-format location pos.
    * @return Returns the color of the piece, possible values are BLANK, BLACK and WHITE.
    * @param pos GUI-format (0..63) board position of the piece to query.
    */
    // get piece at pos, BLANK BLACK WHITE
    unsigned int getPiece(unsigned int pos);

    /**
    * Get color of the player whose turn it is, BLANK means that the game is over.
    * @return Returns the color of the player whose turn it currently is. Possible values BLANK, BLACK and WHITE.
    */
    // get active player, BLANK=gameover BLACK WHITE
    unsigned int getTurn();

    /**
    * Get the score of player of color color.
    * @return Returns the score (amount of pieces of color) of the specified player.
    * @param color Color of player whose score to query. Permitted values are BLACK and WHITE.
    */
    unsigned int getScore(unsigned int color);

    /**
    * Get a list of positions changed since the last move. First item in the list is the new piece's color, followed
    * by GUI-format positions for the changed pieces.
    * @return Returns a QValueListIterator with the last move's color followed by its position and positions of changed pieces.
    */
    // get changed positions, first item in list is the new piece
    QValueList<unsigned int>::iterator getChanges();

    /**
    * Get the current status of the board. Ordered from 0 to 63 according to GUI-format.
    * @return Returns a QValueListIterator with the colors of pieces, ordered from 0 to 63. Possible values BLANK, BLACK and WHITE.
    */
    // get current board status. uses BLANK, BLACK and WHITE from piece.h
    QValueList<unsigned int>::iterator getBoard();    

    /**
    * Converts a GUI-format unsigned int position to TRules-format TCoord position.
    * @return Returns the TCoord that matches parameter pos.
    * @param pos GUI-format (0..63) position to convert to TCoord.
    */
    // convert unsigned int to TCoord
    TCoord uiToTCoord(unsigned int pos);

    /**
    * Converts a TRules-format TCoord position to GUI-format unsigned int position.
    * @return Returns the GUI-format position that matches parameter pos.
    * @param pos TRules-format TCoord position to convert to GUI-format unsigned int.
    */
    // convert TCoord to unsigned int
    unsigned int TCoordToUi(TCoord pos);

  private:
    /**
    * Updates getBoard-list and getChanges-list.
    * @param pos Position of the new move.
    */
    void updateBoard(unsigned int pos);
    /**
    * Updates the turn-property to match current board state.
    */
    void updateTurn();

    auto_ptr< QValueList<unsigned int> > board;
    auto_ptr< QValueList<unsigned int> > changes;

    unsigned int turn;
};

#endif // GAMERULES_H
