/***************************************************************************
                          boardview.h  -  description
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

#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include <memory>
#include <qcanvas.h>
#include <qpoint.h>
#include <qqueue.h>

#include "gamerules.h"

using namespace std;

class QWidget;
class QMouseEvent;
class QTimer;
class Piece;

#define BOTH 999999

/**
* @short Handles othello game GUI.
* @author Ilmari Heikkinen
*
* The BoardView class handles all of the graphical othello board.
* It creates new games, handles player connections and mouse events,
* takes care of updating piece locations, animating flips and sending
* status updates to the main program statusbar.
*/
class BoardView : public QCanvasView
{
  Q_OBJECT
  public:
    /**
    * BoardView constructor. Passes its parameter to the QCanvasView parent classes constructor.
    * Initializes board variables to their default values and starts a new game against computer.
    */
    BoardView( QCanvas *canvas, QWidget *parent, const char *name );
    /**
    * BoardView destructor. Deletes all pointers.
    */
    ~BoardView();

    /**
    * Updates board and calls a redraw method (animateBoard() or clearBoard()) according to the animations-property.
    */
    void updateView();
    /**
    * Checks if there's space for more players on the board.
    * @return TRUE if space exists, FALSE if the board is full.
    */
    bool isSpace();
    /**
    * Connects a player object to the board. This means that the board listens
    * for the player object's signal makeMove(unsigned int) and calls the board
    * makeMove-method to update the board state to match the sent move.
    *
    * @return TRUE if connected successfully, FALSE if connection failed.
    * @param newPlayer Pointer to the player object to connect.
    */
    bool connectPlayer(QObject* newPlayer);
    /**
    * Redraws the board, animates the flips.
    */
    void animateBoard();
    /**
    * Redraws the board without animations.
    */
    void clearBoard();
    /**
    * Set animations-property. The animations-property is used for determining whether
    * or not to animate the flips.
    * @param animate New value to set the animations-property to.
    */
    void setAnimated( bool animate );
    /**
    * Get animations property.
    * @return Returns the value of the animations-property.
    */
    bool getAnimated( );
    /**
    * Set animSpeed-property. The animSpeed-property is used for determining the speed at
    * which to trigger (delay between animation starts) the flip animations.
    * If called with a negative value, sets trigger speed to random.
    * @param new_speed The new speed to which to set the animSpeed-property to.
    */
    void setAnimSpeed(double new_speed);            
    /**
    * Get animSpeed property.
    * @return Returns the value of the animSpeed-property.
    */
    double getAnimSpeed();
    
  protected:
    /**
    * Processes mouseclicks on the board. Calls the makeMove-method with the
    * position clicked and the player-property.
    */
    void contentsMousePressEvent( QMouseEvent* );

    
  protected slots:
    /**
    * Calls the makeMove-method with its parameter. Used for letting connected players to speak to the board.
    * @param unsigned Position of the new move.
    */
    void extMove(unsigned int);

    /**
    * Starts a new game against the computer with the player taking black.
    */
    void newBlackSingleGame();
    /**
    * Starts a new game against the computer with the player taking white.
    */
    void newWhiteSingleGame();
    /**
    * Starts a new two player game.
    */
    void newTwoGame();
    /**
    * Starts a new two player game with modified rules. Flip opposite until blank or own.
    */
    void newWeirdTwoGame();
    /**
    * Starts a new game between two computer players.
    */
    void newDemoGame();

    /**
    * Disconnects a player from the board.
    * @param QObject* Pointer to the player object to be disconnected.
    */
    void disconnectPlayer( QObject* );

    
  signals:
    /**
    * Updates main window statusbar with progress reports from newMove.
    * @param QString& The new statusbar text.
    */
    void moveUpdate( const QString& ); // for statusbar textual update from newMove()
    /**
    * Updates players about new moves.
    * @param unsigned Position of the new move.
    */
    void moveMade( unsigned int ); // signal to players of the location of the move made
            
  private:
    /**
    * Initialize new game.
    * @param new_player The color of the player using the local GUI. BOTH for hotseat two player game.
    * @param weird Whether to play with modified rules or not. Defaults to FALSE.
    */
    // new game initialization
    void newGame(unsigned int new_player, bool weird=FALSE);

    /**
    * Checks move validity and updates the internal board if valid. Calls newMove to take care of updating the GUI board.
    * @return Returns TRUE if the move is valid, returns FALSE if the move is invalid.
    * @param pos The position into which to move.
    */
    // move processing
    bool makeMove(unsigned int pos);
    /**
    * Calls updateBoard to update GUI board. Emits a moveUpdate-signal to update the main window statusbar.
    */
    void newMove();

    /**
    * Disconnects all players (stops listening to their move requests and clears the player list).
    */
    void disconnectAllPlayers();

/**
* Calculates distance of position pos from the latest move.
* @return Returns the distance in number of pieces.
* @param GUI-format (0..63) position to calculate distance with.
*/
double distanceFromNew(unsigned int pos);
/**
* Flip animation start delay algorithms. Calculates distance from new and returns a value based on that.
* @return Returns a value to use as the base delay. Depends on the used algorithm.
* @param algo The algorithm to use. Possible values are 0 (pseudo-random-fixed based on delay), 1 (logarithmic outwards from new piece), 2 (linear outwards from new piece), 3 (logarithmic towards new piece), 4 (linear towards new piece) and 5 (linear outwards from new piece based on angle).
* @param p Pointer to piece to find the start delay for.
* @param delay Pointer to delay value to modify. Currently used only with the 0-algorithm.
*/
double animDelayAlgo(unsigned int algo, Piece* p, double* delay);
/**
* Gets maximum flip distance from new piece on the current board state. Used for doing towards-center timings.
* @return Returns the maximum amount of flips in a single direction possible from the newest move.
*/
double getMaximumDistance();
/**
* Calculates the angle of a given position from the new piece.
* @return Returns the angle from the new piece. Range [0..2PI[
* @param GUI-format (0..63) position to calculate angle with.
*/
double angleFromNew(unsigned int pos);

    /**
    * Animation toggle. When TRUE, gamepiece flips are animated. When FALSE, they're not.
    */
    // animation toggle
    bool animations;
    /**
    * Animation speed, wait animSpeed frames before starting next flip animation in sequence.
    */
    double animSpeed;
    /**
    * Animation random timing toggle. When TRUE, gamepiece flips fire at random intervals.
    * When FALSE, they fire at regular intervals.
    */
    bool animRandomJitter;

    unsigned int animType;
                                
    /**
    * Color of player who uses the GUI.
    */
    unsigned int player;
    /**
    * Remaining player slots on the board.
    */
    unsigned int space; // two minus amt_of_players_connected
    QTimer *Timer;
    /**
    * Internal othello board. The GUI is synchronized against this.
    */
    auto_ptr<GameRules> rules; // internal othello board
    /**
    * List of pointers to connected player objects.
    */
    auto_ptr<QValueList<QObject*> > players; // connected players

};

#endif  // BOARDVIEW_H
