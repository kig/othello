/***************************************************************************
                    playerif.h  -  Player object interface
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

#ifndef PLAYERIFACE_H
#define PLAYERIFACE_H

#include <qobject.h>

/**
* @short Interface for connecting players to the board.
* @author Ilmari Heikkinen
*
* The PlayerIface class is a virtual class that defines the slots and signals
* a Player class must have in order to successfully connect to a BoardView
* game board.
*/  
class PlayerIface : virtual public QObject
{
  Q_OBJECT

  protected slots:
    /**
    * Processes external moves (ie. moves coming from other players on the game board.
    * Should be called when the game board updates with a new move.
    * @param unsigned The position the new move was made into. In GUI-format (0..63).
    */
    virtual void extMove( unsigned int ) = 0;
    
  signals:
    /**
    * Requests the game board to put a piece to position defined by the parameter.
    * @param unsigned The position in which a new move should be made into by the connected game board.
    */
    void makeMove( unsigned int );
    /**
    * Requests the game board to disconnect the specified player object. Usually a reference to self.
    * @param QObject* Pointer to player object to disconnect.
    */
    void disconnectMe( QObject* );

};

#endif // PLAYERIFACE_H
