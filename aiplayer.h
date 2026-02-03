/***************************************************************************
           aiplayer.h  -  computer player class for othello
                             -------------------
    begin                : Mon May 5 2003
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


#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "playerif.h"

class GameRules;
class TRules;
class QTimer;

/**
* @short A computer player class to connect to BoardView
* @author Ilmari Heikkinen
*
* The AIPlayer class communicates with a BoardView class its connected to
* and plays othello according to the AI module implemented.
*/
class AIPlayer : virtual public PlayerIface
{
  Q_OBJECT
  public:
    /**
    * Constructor, sets player color, opposing player color and connects the timer
    * to the AI move processor.
    * @param color The color the AI should play. Permitted values are BLACK and WHITE.
    */
    AIPlayer(unsigned int color);
    /**
    * Destructor, deletes pointers.
    */
    ~AIPlayer();

  protected slots:
    /**
    * Implemented from PlayerIface. Should be connected to BoardView new move signal.
    * @param unsigned GUI-format (0..63) position of the new move.
    */
    virtual void extMove( unsigned int );
    /**
    * The actual move processing function. Asks the AI module for a move and then fires
    * off a makeMove(unsigned) signal that should be listened to by the BoardView the
    * player is connected to. Is connected to gTimer to delay move processing.
    */
    void processMove();
    
  private:
    /**
    * Internal othello board for keeping tab of the game.
    */
    GameRules* rules; // internal othello board
    /**
    * For AI module compatibility.
    */
    TRules* trules; // ai compatibility
    /**
    * Timer for move processing.
    */
    QTimer* gTimer;
    /**
    * Color of the AI player.
    */
    unsigned int color;
    /**
    * Color of the opposing player.
    */
    unsigned int opposite;
    /**
    * Stores where the last move received was made.
    */
    unsigned int last_pos;
};

#endif // AIPLAYER_H
