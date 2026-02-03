/***************************************************************************
                          othellocanvasview.cpp  -  description
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

#include "piece.h"

#include <qcanvas.h>
#include <qstring.h>
#include <qimage.h>

Piece::Piece( QCanvasPixmapArray *parray, QCanvas *canvas ) : QCanvasSprite( parray, canvas ), color(BLANK)
{
  setZ( 1 );
  setAnimated( false );     // disallow animation
}

Piece::~Piece()
{}

void Piece::flip()
{
  if (color == BLACK) {
      setFrame( WHITE );   
  } else {
      setFrame( BLACK );
  }
  color = (color == BLACK) ? WHITE : BLACK;
}

// returns true if successful (new position within limits), otherwise false
bool Piece::setPosition(unsigned int new_position)
{
  if (new_position < (COLUMNS*ROWS)) {
      position = new_position;
      // move piece on board, x * square_width, y * square_height
      double x = (position % COLUMNS);
      double y = (position / ROWS);

      move(x*SQUARE_W+1, y*SQUARE_H+1);
      setZ( position );
      return true;
  }
  return false;
}

int Piece::rtti() const {
  return PIECE_RTTI;
}

unsigned int Piece::getPosition()
{
  return position;
}

void Piece::setColor( unsigned int new_color )
{
  if (new_color == BLACK) {
    color = BLACK;
  } else if (new_color == WHITE) {
    color = WHITE;
  } else {
    color = BLANK;
  }
}

unsigned int Piece::getColor()
{
  return color;
}
