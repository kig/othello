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

#include "boardview.h"
#include "piece.h"
#include "animpiece.h"
#include "gamerules.h"

#include "aiplayer.h"

#include <qwidget.h>
#include <qevent.h>
#include <qtimer.h>


////////
// PUBLIC METHODS
//

BoardView::BoardView( QCanvas *canvas, QWidget *parent, const char *name )
    : QCanvasView( canvas, parent, name ),
    rules(new GameRules()),
    players(new QValueList<QObject*>)
{
  animations=TRUE;
  animSpeed=1;
  animRandomJitter=FALSE;

  newMove();
  newBlackSingleGame();

  animType = 2;
}

BoardView::~BoardView()
{
  // tell the player objects to bugger off and die
  disconnectAllPlayers();
  delete Timer;
}


// update status of pieces on board
// animate if animations
void BoardView::updateView()
{
  if (animations) {
      animateBoard();
  } else {
      clearBoard();
  }
  canvas()->update();
}

// update board with flip animations
void BoardView::animateBoard()
{
    QCanvasItemList list = canvas()->allItems();
    QCanvasItemList::iterator it, jt;
    double i=-1;
    double delay = 0;
    for ( it = list.begin(); it != list.end(); ++it) {
      if ( (*it)->rtti() == PIECE_RTTI )
      {
        Piece* p = (Piece*) (*it);

        // the color of piece in internal othello board
        unsigned int newFrame = rules->getPiece(  p->getPosition()  );

        if ( p->getColor() != newFrame ) {
          if (newFrame == BLANK || p->getColor() == BLANK) {
            p->setColor(newFrame);
            p->setFrame(newFrame);
          } else {
            for ( jt = list.begin(); jt != list.end(); ++jt) {
                // find animation that matches the new state of the piece
                if ( (*jt)->rtti() == ANIM_RTTI ) {
                    AnimPiece* a = (AnimPiece*) (*jt);
                    // correctly colored and in the correct position
                    if ( (a->getResult() == newFrame) && (a->getPosition() == p->getPosition()) ) {
                      if (!animRandomJitter) {
                        i = animSpeed * animDelayAlgo(animType, p, &delay);
                      } else {
                        i++;
                      }
                      a->animPlay(p,i,animRandomJitter);
                      break;
                    }
                }
            }
          }
        }
      }
    }
}

double BoardView::animDelayAlgo(unsigned int algo, Piece* p, double* delay)
{
  switch (algo) {
    case 1: // logarithmic away from center
      return ( 5.5*(log10(3.5*distanceFromNew(p->getPosition()) - 3.0)) );
      break;
    case 2: // linear away from center
      return ( distanceFromNew(p->getPosition()) - 1.0);
      break;
    case 3: // logarithmic towards center
      return (4*getMaximumDistance())/( 5.5*(log10(3.5*distanceFromNew(p->getPosition()) - 3.0)) );
      break;
    case 4: // linear towards center
      return (3*getMaximumDistance())/(1 + abs( distanceFromNew(p->getPosition()))) - 1.0;
      break;
    case 5: // linear away from center, angled
      return (distanceFromNew(p->getPosition()) -1.0) + M_PI + angleFromNew(p->getPosition());
      break;
    default:
      double retval = (*delay);
      (*delay)++;
      return (retval);
  }
}

// angle from new piece
double BoardView::angleFromNew(unsigned int pos)
{
  QValueList<unsigned int>::iterator changes = rules->getChanges();
  changes++;
  unsigned int newpos = (*changes);
  TCoord p = rules->uiToTCoord(pos);
  TCoord n = rules->uiToTCoord(newpos);

  double diff_x = p.x - n.y;
  double diff_y = p.y - n.y;

  double angle = atan2(diff_x, diff_y);
  return angle;
}

// maximum distance from new piece
double BoardView::getMaximumDistance()
{
  double maxDistance=0, temp=0;
  QValueList<unsigned int>::iterator changes = rules->getChanges();
  changes++;
  changes++;
  do {
      temp=distanceFromNew((*changes));
      if (temp > maxDistance) {
              maxDistance = temp;
      }
      changes++;
  } while ((*changes) != 0 && (*changes) < 64);
  return maxDistance;
}

// distance from new piece in pieces
double BoardView::distanceFromNew(unsigned int pos)
{
  QValueList<unsigned int>::iterator changes = rules->getChanges();
  changes++;
  unsigned int newpos = (*changes);
  TCoord p = rules->uiToTCoord(pos);
  TCoord n = rules->uiToTCoord(newpos);

  // double diff  = sqrt( pow((float)p.x-n.x,2) + pow((float)p.y-n.y,2) );
  double abs_x = abs(p.x - n.x);
  double abs_y = abs(p.y - n.y);
  return (abs_x < abs_y) ? abs_y : abs_x;
}

// set piece frames and colors according to current state, no animations
void BoardView::clearBoard()
{
  QCanvasItemList list = canvas()->allItems();
  QCanvasItemList::iterator it;
  for ( it = list.begin(); it != list.end(); ++it) {
    if ( (*it)->rtti() == PIECE_RTTI ) {
      unsigned int newFrame = rules->getPiece( ((Piece*)(*it))->getPosition() );
      ((Piece*)(*it))->setColor( newFrame );
      ((Piece*)(*it))->setFrame( newFrame );
    } else if ( (*it)->rtti() == ANIM_RTTI ) {
      ((AnimPiece*)(*it))->stopPlaying();
    }
  }
}

// return true if there's space for another player, else false
bool BoardView::isSpace()
{
  if (space > 0) {
    return true;
  } else {
    return false;
  }
}
                                      
// connect newPlayer if there's space,
// listen to newPlayer's makeMove-signal
bool BoardView::connectPlayer(QObject* newPlayer)
{
  if (isSpace()) {
        space--;
        players->append(newPlayer);
        // listen to the moves the new player makes
        connect (newPlayer, SIGNAL( makeMove(unsigned int) ),
                      this, SLOT( extMove(unsigned int) ));
        // listen to the death throes of the player
        connect (newPlayer, SIGNAL( disconnectMe(QObject*) ),
                      this, SLOT( disconnectPlayer(QObject*) ));
        return true;
       
  } else {
        return false;
  }
}

// set animation variable
void BoardView::setAnimated(bool animate)
{
  animations = animate;
}
// get animation variable
bool BoardView::getAnimated()
{
  return animations;
}
// set animation speed. if new speed is negative, use random jitter
void BoardView::setAnimSpeed(double new_speed)
{
  if (new_speed >= 0  ) {
    animSpeed = new_speed;
    animRandomJitter = FALSE;
  } else {
    animSpeed = 1;
    animRandomJitter = TRUE;
  }
}
// get animation speed
double BoardView::getAnimSpeed()
{
  return animSpeed;
}


////////
// PROTECTED METHODS
//

// process mouseclicks
void BoardView::contentsMousePressEvent( QMouseEvent *e )
{
    // a list of all items underneath the mouse when it was pressed
    QCanvasItemList list = canvas()->collisions(e->pos());

    // RTTI of a QCanvasSprite is 1
    if ( !list.isEmpty() && list.first()->rtti() == PIECE_RTTI )
    {
       Piece* p = (Piece*)list.first();
       if (p->frame() == BLANK) {
         unsigned int pos = p->getPosition();
         if (player != BLANK &&
             (player == BOTH || rules->getTurn() == player) &&
             makeMove(pos)
            ) { // something to process after move made?
         }
       }
    }    
}


////////
// PROTECTED SLOTS
//

// process external move calls
void BoardView::extMove(unsigned int pos) {
  if (rules->getTurn() != player || // not local player's turn, ie. external player's turn
      player == BLANK) { // [computer vs computer] or [network client vs network client]
      makeMove(pos);
  }
}

// gametype call wrappers for the menu
// set default amount of space for gametype, create game

// play with black, computer is white
void BoardView::newBlackSingleGame()
{
  disconnectAllPlayers();
  space = 1;

  AIPlayer* ai = new AIPlayer(WHITE);
  connectPlayer(ai);
  connect( this, SIGNAL(moveMade(unsigned int)), ai, SLOT(extMove(unsigned int)));

  newGame(BLACK);
}

// play with white, computer is black
void BoardView::newWhiteSingleGame()
{
  disconnectAllPlayers();
  space = 1;

  AIPlayer* ai = new AIPlayer(BLACK);
  connectPlayer(ai);
  connect( this, SIGNAL(moveMade(unsigned int)), ai, SLOT(extMove(unsigned int)));

  newGame(WHITE);
}

// play a two player game with regular rules
void BoardView::newTwoGame()
{
  disconnectAllPlayers();
  space = 0;

  newGame(BOTH);
}

// play a two player game with flip adjacent opposites until blank or own -rules
void BoardView::newWeirdTwoGame()
{
  disconnectAllPlayers();
  space = 0;

  newGame(BOTH, TRUE);
}

// computer vs computer
void BoardView::newDemoGame()
{
  disconnectAllPlayers();
  space = 2;

  AIPlayer* ai = new AIPlayer(WHITE);
  connectPlayer(ai);
  connect( this, SIGNAL(moveMade(unsigned int)), ai, SLOT(extMove(unsigned int)));
  ai = new AIPlayer(BLACK);
  connectPlayer(ai);
  connect( this, SIGNAL(moveMade(unsigned int)), ai, SLOT(extMove(unsigned int)));

  newGame(BLANK);
}

// disconnects player
void BoardView::disconnectPlayer(QObject* exPlayer) {
       disconnect (exPlayer, SIGNAL( disconnectMe(QObject*) ),
                       this, SLOT( disconnectPlayer(QObject*) ));
       disconnect (exPlayer, SIGNAL( makeMove(unsigned int) ),
                       this, SLOT( extMove(unsigned int) ));
       disconnect (this, SIGNAL(moveMade(unsigned int)), exPlayer, SLOT(extMove(unsigned int)));
}



////////
// PRIVATE METHODS
//

// set player (one who clicks on the board), clear board, update view
// player values: BLACK, WHITE, BOTH (two player game), BLANK (ai vs ai)
void BoardView::newGame(unsigned int new_player, bool weird)
{
  player = new_player;
  
  // ditch old rules, make new ones
  std::auto_ptr<GameRules>temp = rules;
  rules = std::auto_ptr<GameRules>( new GameRules(weird) );

  // clear board and update
  clearBoard();
  newMove();
  
  // tell ai to start
  if (player != BLACK) { emit moveMade(0); }
}


// disconnects all player objects
void BoardView::disconnectAllPlayers () {
  QValueList<QObject*>::iterator it;
  for (it = players->begin(); it != players->end(); ++it) {
      disconnectPlayer((*it));
      delete (*it);
  }
  // clear player list
  players->clear();
}


// make move to position pos, tell players move was made
bool BoardView::makeMove(unsigned int pos)
{
  bool retval = rules->setPiece(pos);
  if (retval) {
      newMove();
      emit moveMade( pos ); // tells about the move
  }
  return retval;
}


// do whatever needs to be done when a new move is made
void BoardView::newMove()
{
  updateView();

  // build statusbar signal message
  QString msg;
  if (rules->getTurn() != BLANK) {
    if (rules->getTurn() == BLACK) {
      msg = "Black's";
    } else if (rules->getTurn() == WHITE) {
      msg = "White's";
    }
    if (rules->getTurn() == player) {
      msg += " (your)";
    }
    msg += " turn.";
  } else {
    unsigned int b_score, w_score;
    b_score = rules->getScore(BLACK);
    w_score = rules->getScore(WHITE);

    msg = "Game over! ";

    if (b_score > w_score) {
      msg += "Black wins";
    } else if (b_score < w_score) {
      msg += "White wins";
    } else {
      msg += "Tie";
    }

    msg += " (";
    msg += QString().setNum(b_score);
    msg += " - ";
    msg += QString().setNum(w_score);
    msg += ").";

  }
  emit moveUpdate( msg );
}

