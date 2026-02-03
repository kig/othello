/***************************************************************************
                          animpiece.cpp  -  description
                             -------------------
    begin                : Sat May 10 2003
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

#include "animpiece.h"

#include <qtimer.h>
#include <qobject.h>
#include <qdatetime.h>
#include <stdlib.h>
#include <time.h>

// connect timers to animation start and frame advance, set direction
AnimPiece::AnimPiece( QCanvasPixmapArray *parray, QCanvas *canvas, unsigned int d ) :
 QObject(), Piece(parray,canvas), direction(d), playing(FALSE), STimer(new QTimer), Timer(new QTimer)
{
    // set position out of range to make position updating mandatory on first time
    position = ROWS*COLUMNS;
    // seed rand() with processor ticks
    srand( (int)clock() );
    
    connect( STimer, SIGNAL(timeout()), SLOT(startAnim()) );
    connect( Timer, SIGNAL(timeout()), SLOT(advanceFrame()) );
}

AnimPiece::~AnimPiece()
{
  STimer->stop();
  Timer->stop();
  delete STimer;
  delete Timer;
}

// move to new position. z-index according to color
bool AnimPiece::setPosition(unsigned int pos)
{
  if (pos < (64)) { /*COLUMNS*ROWS*/
      if (position != pos) {
        position = pos;
        double x_offset, y_offset;
        if (direction == WTB) {
            x_offset = WTB_X_OFFSET;
            y_offset = WTB_Y_OFFSET;
          } else {
            x_offset = BTW_X_OFFSET;
            y_offset = BTW_Y_OFFSET;
          }

          // move piece on board, x * square_width, y * square_height. plus offsets
          double x = (double)(position % COLUMNS);
          double y = (double)(position / ROWS);
          move((x*SQUARE_W)+(x_offset), (y*SQUARE_H)+(y_offset));
      }      
      int z;
      z = 100 + position;
      setZ( z );
      return true;
  }
  return false;
}

// set direction. unused.
void AnimPiece::setDirection(unsigned int d)
{
 direction = d;
}

// identifier for canvasitems
int AnimPiece::rtti() const {
  return ANIM_RTTI;
}

// returns true if the animation is playing
bool AnimPiece::isPlaying()
{
  return playing;
}

// returns the result color of the flip animation
unsigned int AnimPiece::getResult()
{
  if (direction == BTW) {
    return WHITE;
  } else {
    return BLACK;
  }
}

// set next frame unless last, in which case hide animation
// and set the piece to right color
void AnimPiece::advanceFrame()
{
  if (currentFrame < FRAMES) {
    if (currentFrame == 0) {
      show();
    }
    setFrame(currentFrame);
    currentFrame++;
  } else {
    piece->setFrame( getResult() );
    stopPlaying();
  }  
}

// stop frame timer, rewind, show resulting piece frame, hide anim
void AnimPiece::stopPlaying()
{
    playing = FALSE;
    currentFrame = 0;

    Timer->stop();
    setFrame(currentFrame);
    hide();
}

// stop animstart timer, reset animation, set piece, start playing
void AnimPiece::animPlay(Piece* p, double seq, bool jitter)
{
  STimer->stop();
  Timer->stop();
  playing = TRUE;  
  piece = p;
  setPosition(p->getPosition());
  piece->setColor( getResult() );
  // anim start time jitter for ze bombardment.
  int startDelay;
  if (jitter && seq > 0) { // fire off first in sequence instantly
    startDelay = (int)(((float)rand()/(float)0x7fffffff)*JITTER*FRAME_DELAY);
  } else {
    startDelay = (int)(FRAME_DELAY * seq);
  }
  STimer->start( startDelay, TRUE );
}

void AnimPiece::startAnim() {
  Timer->start( FRAME_DELAY, FALSE );
  advanceFrame();
}
