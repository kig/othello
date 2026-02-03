/***************************************************************************
                          othello.cpp  -  description
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

#include <qpopupmenu.h>
#include <qstatusbar.h>
#include <qmenubar.h>
#include <qcanvas.h>
#include <qpixmap.h>
#include <qlayout.h>
#include <qvbox.h>
#include <qmessagebox.h>

#include "othello.h"
#include "animpiece.h"
#include "piece.h"
#include "boardview.h"
#include "gamerules.h"
#include "rules.h"

/**
* Othello program main window. Creates a new QCanvas object to
* act as the playing board and sets its properties.
* Creates a new BoardView (inherited from QCanvasView) to make
* the Canvas visible. Initializes gamepiece sprites and their
* animations for inclusion in the Canvas. Creates menus and
* connects the menuitems to related slots. Creates the statusbar
* and sets its properties. Finally, composes the window from the
* generated parts.
*/
Othello::Othello(const char *name) : QMainWindow(0L , name)
{
    // Create and setup canvas and view
    Canvas = new QCanvas( this, "Canvas" );
    Canvas->setBackgroundColor( Qt::black );
    Canvas->setBackgroundPixmap( QPixmap(  DEFAULT_BG ) );
    Canvas->setAdvancePeriod( 10 );
    Canvas->resize( 450, 450 );
    OthelloBoard = new BoardView( Canvas, this, "OthelloBoard");

    // setup list of gamepiece frames
    QValueList<QPixmap> list;
    list.append(QPixmap(  DEFAULT_BLANK ));
    list.append(QPixmap(  DEFAULT_BLACK ));
    list.append(QPixmap(  DEFAULT_WHITE ));

    // create pixmaparray of gamepiece frames
    pieces = new QCanvasPixmapArray (list);

    // create gamepiece list
    for (int i=0; i<COLUMNS*ROWS; i++) {
      piecelist.append( new Piece(pieces,Canvas) );
      piecelist.last()->setPosition( i );
      piecelist.last()->setFrame( BLANK );
      piecelist.last()->show();
    }

    // setup list of animpiece animation frames
    // white-to-black
    QValueList<QPixmap> wtblist;
    wtblist.append(QPixmap(  DEFAULT_WTB_1 ));
    wtblist.append(QPixmap(  DEFAULT_WTB_2 ));
    wtblist.append(QPixmap(  DEFAULT_WTB_3 ));
    wtblist.append(QPixmap(  DEFAULT_WTB_4 ));
    wtblist.append(QPixmap(  DEFAULT_WTB_5 ));
    wtblist.append(QPixmap(  DEFAULT_WTB_6 ));
    wtblist.append(QPixmap(  DEFAULT_WTB_7 ));
    // black-to-white
    QValueList<QPixmap> btwlist;
    btwlist.append(QPixmap(  DEFAULT_BTW_1 ));
    btwlist.append(QPixmap(  DEFAULT_BTW_2 ));
    btwlist.append(QPixmap(  DEFAULT_BTW_3 ));
    btwlist.append(QPixmap(  DEFAULT_BTW_4 ));
    btwlist.append(QPixmap(  DEFAULT_BTW_5 ));
    btwlist.append(QPixmap(  DEFAULT_BTW_6 ));
    btwlist.append(QPixmap(  DEFAULT_BTW_7 ));

    // create pixmaparrays of animpiece frames
    wtbpieces = new QCanvasPixmapArray(wtblist); 
    btwpieces = new QCanvasPixmapArray(btwlist);

    // create animpiece lists
    for (int i=0; i<COLUMNS*ROWS; i++) {
      wtbpiecelist.append( new AnimPiece(wtbpieces,Canvas,WTB) );
      wtbpiecelist.last()->setPosition( i );
      btwpiecelist.append( new AnimPiece(btwpieces,Canvas,BTW) );
      btwpiecelist.last()->setPosition( i );
    }

    OthelloBoard->clearBoard();
  
    // FILE menu
    QPopupMenu * filemenu = new QPopupMenu;
    filemenu->insertItem( "&Black Vs. Computer", OthelloBoard, SLOT( newBlackSingleGame() ) );
    filemenu->insertItem( "&White Vs. Computer", OthelloBoard, SLOT( newWhiteSingleGame() ) );
    filemenu->insertSeparator();
    filemenu->insertItem( "&Two Player Game", OthelloBoard, SLOT( newTwoGame() ) );
    filemenu->insertItem( "&Weird Two Player Game", OthelloBoard, SLOT( newWeirdTwoGame() ) );
    filemenu->insertSeparator();
    filemenu->insertItem( "&View Demo Game", OthelloBoard, SLOT( newDemoGame() ) );
/*
    filemenu->insertSeparator();
    filemenu->insertItem( "&Host Network Game" );
    filemenu->insertItem( "&Join Network Game" );
*/
    filemenu->insertSeparator();
    filemenu->insertItem( "&Quit", qApp, SLOT( quit() ) );

    // OPTIONS menu
    optionsmenu = new QPopupMenu;
    anim_id = optionsmenu->insertItem( "&Animated flips", this, SLOT( toggleAnim() ) );
    optionsmenu->setItemChecked(anim_id, TRUE);
    animenu = new QPopupMenu;
    animSpeeds.append( animenu->insertItem( "&Fastest", this, SLOT(setAnimSpeed1()) )  );
    animSpeeds.append( animenu->insertItem( "&Fast", this, SLOT(setAnimSpeed2()) )  );
    animSpeeds.append( animenu->insertItem( "&Normal", this, SLOT(setAnimSpeed3()) )  );
    animSpeeds.append( animenu->insertItem( "&Slow", this, SLOT(setAnimSpeed4()) )  );
    animSpeeds.append( animenu->insertItem( "&Slowest", this, SLOT(setAnimSpeed5()) )  );
    animenu->insertSeparator();
    animSpeeds.append( animenu->insertItem( "&Random", this, SLOT(setAnimSpeedR()) )  );
    optionsmenu->insertItem( "&Animation speed", animenu );

    setAnimSpeed3();
        
    // HELP menu
    QPopupMenu *helpmenu = new QPopupMenu( this,"HelpPopupMenu" );
    helpmenu->insertItem( "&About", this, SLOT(about()));

    // insert items into menu
    QMenuBar * menu = menuBar();
    menu->insertItem( "&Game", filemenu );
    // menu->insertSeparator();
    menu->insertItem( "&Options", optionsmenu );
    menu->insertSeparator();
    menu->insertItem( "&Help", helpmenu );

    // STATUSBAR
    QStatusBar *statusbar = statusBar();
    connect( OthelloBoard, SIGNAL(moveUpdate(const QString&)),
                 statusbar, SLOT(message(const QString&)) );
    statusbar->setSizeGripEnabled(FALSE);

                 
    setCentralWidget( OthelloBoard );
}

/**
* Clears the piecelists and then proceeds to delete all pointers.
*/
Othello::~Othello()
{
    while (!piecelist.isEmpty()) {
          delete piecelist.last();
          piecelist.pop_back();
    }
    while (!wtbpiecelist.isEmpty()) {
          delete wtbpiecelist.last();
          wtbpiecelist.pop_back();
    }
    while (!btwpiecelist.isEmpty()) {
          delete btwpiecelist.last();
          btwpiecelist.pop_back();
    }

    delete animenu;
    delete optionsmenu;
    delete wtbpieces;
    delete btwpieces;
    delete pieces;
    delete OthelloBoard;
    delete Canvas;
}

/**
* Pop up an about window.
*/
void Othello::about()
{
    QMessageBox::about( this, "Othello v0.9",
                        "A Qt Othello Game.\n\n"
                        "Ilmari Heikkinen (irheikki@cc.hut.fi)\nJari Väisänen (jvaisan2@cc.hut.fi)");
}

/**
* Toggles animations on and off using BoardView's setAnimated(bool) method.
*/
void Othello::toggleAnim()
{
    bool s = !optionsmenu->isItemChecked(anim_id);
    optionsmenu->setItemChecked(anim_id,s);
    OthelloBoard->setAnimated(s);
}
/**
* Sets BoardView's animation speed to fastest.
*/
void Othello::setAnimSpeed1()
{
checkAnimSpeed(0);
OthelloBoard->setAnimSpeed(0);
}
/**
* Sets BoardView's animation speed to fast.
*/
void Othello::setAnimSpeed2()
{
checkAnimSpeed(1);
OthelloBoard->setAnimSpeed(0.5);
}
/**
* Sets BoardView's animation speed to normal.
*/
void Othello::setAnimSpeed3()
{
checkAnimSpeed(2);
OthelloBoard->setAnimSpeed(1);
}
/**
* Sets BoardView's animation speed to slow.
*/
void Othello::setAnimSpeed4()
{
checkAnimSpeed(3);
OthelloBoard->setAnimSpeed(2);
}
/**
* Sets BoardView's animation speed to slowest.
*/
void Othello::setAnimSpeed5()
{
checkAnimSpeed(4);
OthelloBoard->setAnimSpeed(3);
}
/**
* Sets BoardView's animation speed to random.
*/
void Othello::setAnimSpeedR()
{
checkAnimSpeed(5);
OthelloBoard->setAnimSpeed(-1);
}

/**
* Sets the Animation Speed -menus i'th (0-6) menuitem checked.
* Clears other checkmarks from the Animation Speed -menu.
*/
void Othello::checkAnimSpeed(unsigned int i)
{
  QValueList<int>::iterator it;
  for (it = animSpeeds.begin(); it != animSpeeds.end(); ++it) {
      animenu->setItemChecked((*it), FALSE);
  }
  animenu->setItemChecked(animSpeeds[i], TRUE);
}

