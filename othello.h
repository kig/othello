/***************************************************************************
                          othello.h  -  description
                             -------------------
    begin                : Sat May  3 13:55:00 EEST 2003
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

#ifndef OTHELLO_H
#define OTHELLO_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qmainwindow.h>
#include <qapplication.h>
#include <qwidget.h>
#include <qvaluelist.h>
#include <memory>
#include "piece.h"
#include "animpiece.h"

#define DEFAULT_BG "pics/board.png"

class QCanvas;
class BoardView;
class GameRules;
class Rules;
class QCanvasPixmapArray;

/**
* @short Othello program main window.
* @author Ilmari Heikkinen
*
* The Othello class handles initialization tasks for the game. It sets up the board,
* the pieces, loads animations and creates menus and the statusbar.
*
* The Othello class also handles processing menu events and relaying them
* to the actual game.
* 
*/
class Othello : public QMainWindow
{
  Q_OBJECT 
  public:
/**
* Othello program main window constructor
*
* Creates a new QCanvas object Canvas to
* act as the playing board and sets its properties.
*
* Creates a new BoardView (inherited from QCanvasView) OthelloBoard to make
* the Canvas visible.
*
* Initializes gamepiece sprites and their
* animations for inclusion in the Canvas.
*
* Creates menus and
* connects the menuitems to related slots.
*
* Creates the statusbar
* and sets its properties.
*
* Finally, composes the window from the
* generated parts.
*/
    Othello(const char *name=0);
/**
* Othello program main window destructor.
*
* Clears the piecelists and then proceeds to delete all pointers.
*/
    ~Othello();

  protected slots:
/**
* Pop up an about window.
*/
    void about();
/**
* Toggles animations on and off using BoardView's setAnimated(bool) method.
*/
    void toggleAnim();
/**
* Sets OthelloBoard's animation speed to fastest.
*/
    void setAnimSpeed1();
/**
* Sets OthelloBoard's animation speed to fast.
*/
    void setAnimSpeed2();
/**
* Sets OthelloBoard's animation speed to normal.
*/
    void setAnimSpeed3();
/**
* Sets OthelloBoard's animation speed to slow.
*/
    void setAnimSpeed4();
/**
* Sets OthelloBoard's animation speed to slowest.
*/
    void setAnimSpeed5();
/**
* Sets OthelloBoard's animation speed to random.
*/
    void setAnimSpeedR();
                
  private:
/**
* Sets the Animation Speed -menus i'th (0-5) menuitem checked.
* Clears other checkmarks from the Animation Speed -menu.
*/
    void checkAnimSpeed(unsigned int i);

/**
* Othello board canvas
*/
    QCanvas* Canvas;
/**
* For showing Canvas and all actual game logic processing.
*/
    BoardView* OthelloBoard;
    QCanvasPixmapArray* pieces;
    QCanvasPixmapArray* wtbpieces;
    QCanvasPixmapArray* btwpieces;
/**
* List of pieces on canvas. 64 in total on a 8x8 board.
*/
    QValueList<Piece*> piecelist;
/**
* List of piece animations (white-to-black) on canvas. 64 in total on a 8x8 board.
*/
    QValueList<AnimPiece*> btwpiecelist;
/**
* List of piece animations (black-to-white) on canvas. 64 in total on a 8x8 board.
*/
    QValueList<AnimPiece*> wtbpiecelist;
/**
* List of references to Animation Speed menuitems.
*/
    QValueList<int> animSpeeds;
        
/**
* Options menu.
*/
    QPopupMenu* optionsmenu;
/**
* Animation speed menu.
*/
    QPopupMenu* animenu;
    
/**
* Reference to Animated flips menuitem.
*/
    int anim_id;
};

#endif
