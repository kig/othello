/***************************************************************************
                          piece.h  -  description
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

#ifndef PIECE_H
#define PIECE_H

#include <qstring.h>
#include <qcanvas.h>
#include <qpoint.h>

// othello board and piece defines
#define DEFAULT_BLANK "pics/blank.png"
#define DEFAULT_BLACK "pics/black.png"
#define DEFAULT_WHITE "pics/white.png"

#define BLANK 0
#define BLACK 1
#define WHITE 2

#define ROWS 8
#define COLUMNS 8

#define SQUARE_W 56
#define SQUARE_H 56

#define PIECE_RTTI 1001

/**
* @short A two-color (plus transparent) sprite for representing a single othello board position.
* @author Ilmari Heikkinen
*
* The Piece-class represents a single othello board position. By default its set to transparent - 
* BLANK - frame. It defines default piece image locations on disk DEFAULT_BLANK, DEFAULT_BLACK and
* DEFAULT_WHITE. It also defines the corresponding frame numbers BLANK, BLACK and WHITE and the piece
* dimensions SQUARE_W and SQUARE_H that are used for computing the pixel position of the piece on board.
*/
class Piece : public QCanvasSprite
{
public:
    Piece( QCanvasPixmapArray *parray, QCanvas *canvas );
    ~Piece();

    /**
    * Flips the piece. Sets piece color to WHITE if the current color is BLACK. Sets piece color to BLACK otherwise.
    */
    void flip();
    /**
    * Sets piece position on board and moves it there.
    * Uses SQUARE_W and SQUARE_H for computing the new location coordinates.
    * @param new_position The position to move the piece to. In GUI-format unsigned int (0..63).
    * @return Returns FALSE if new_position is illegal, TRUE otherwise.
    */
    bool setPosition( unsigned int new_position );
    /**
    * Gets PIECE_RTTI to identify a Piece from other QCanvasSprites
    * @return Returns PIECE_RTTI
    */
    int rtti() const;
    /**
    * Get current piece position.
    * @return Returns the piece position in GUI-format (0..63).
    */
    unsigned int getPosition();
    /**
    * Sets piece color to new_color.
    * @param new_color Color to set the piece color to. Permitted values are BLANK, BLACK and WHITE.
    */
    void setColor( unsigned int new_color );
    /**
    * Get piece color.
    * @return Returns current piece color. Possible values are BLANK, BLACK and WHITE.
    */
    unsigned int getColor();
                                
protected:
    /**
    * Minimum position.
    */
    unsigned int min_pos;
    /**
    * Maximum position.
    */
    unsigned int max_pos;
    /**
    * Current position.
    */
    unsigned int position;
    /**
    * Current color.
    */
    unsigned int color;

};


#endif  // PIECE_H
