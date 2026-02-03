/***************************************************************************
                          animpiece.h  -  description
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

#ifndef ANIMPIECE_H
#define ANIMPIECE_H

#include "piece.h"

// animation frame-to-frame delay (in ms) and start jitter (multiples of FRAME_DELAY)
#define FRAME_DELAY 40
#define JITTER 6

// white-to-black & black-to-white defs
#define WTB 1
#define BTW 0

#define WTB_X_OFFSET -51
#define WTB_Y_OFFSET -51
#define BTW_X_OFFSET -51
#define BTW_Y_OFFSET -51

#define DEFAULT_WTB_1 "pics/wtb/white_to_black1.png"
#define DEFAULT_WTB_2 "pics/wtb/white_to_black2.png"
#define DEFAULT_WTB_3 "pics/wtb/white_to_black3.png"
#define DEFAULT_WTB_4 "pics/wtb/white_to_black4.png"
#define DEFAULT_WTB_5 "pics/wtb/white_to_black5.png"
#define DEFAULT_WTB_6 "pics/wtb/white_to_black6.png"
#define DEFAULT_WTB_7 "pics/wtb/white_to_black7.png"

#define DEFAULT_BTW_1 "pics/btw/black_to_white1.png"
#define DEFAULT_BTW_2 "pics/btw/black_to_white2.png"
#define DEFAULT_BTW_3 "pics/btw/black_to_white3.png"
#define DEFAULT_BTW_4 "pics/btw/black_to_white4.png"
#define DEFAULT_BTW_5 "pics/btw/black_to_white5.png"
#define DEFAULT_BTW_6 "pics/btw/black_to_white6.png"
#define DEFAULT_BTW_7 "pics/btw/black_to_white7.png"

#define FRAMES 7

#define ANIM_RTTI 1002

#include <qobject.h>

class QTimer;

/**
* @short Animated extension to Piece-class, also inheriting QObject to handle signals and slots.
* @author Ilmari Heikkinen
*
* The AnimPiece-class inherits both Piece and QObject in order to have both the properties of a
* QCanvasSprite and the signal-slot-communication of a QObject. The AnimPiece class defines
* FRAME_DELAY, JITTER factor (multiples of FRAME_DELAY) and default locations on disk for black-to-white
* and white-to-black flip animation frames (DEFAULT_WTB_1..DEFAULT_WTB_7, DEFAULT_BTW_1..DEFAULT_BTW_7).
*
* Animating is handled by changing the AnimPiece frame to the tune of QTimer* Timer, and animation start
* delay is handled by QTimer* STimer.
*/
class AnimPiece : public QObject, public Piece
{
Q_OBJECT
public:
    /**
    * Constructor, calls Piece constructor with the first two parameters.
    * Seeds random number generator, connects timers to slots.
    * @param d Direction of the piece animation, BTW (black to white) or WTB (white to black).
    */
    AnimPiece( QCanvasPixmapArray *parray, QCanvas *canvas, unsigned int d );
    /**
    * Destructor, deletes timer pointers.
    */
    ~AnimPiece();

    /**
    * Overloaded version of the Piece class method.
    */
    bool setPosition(unsigned int);
    /**
    * Sets animation direction.
    * @param unsigned New direction, permitted values BTW and WTB.
    */
    void setDirection(unsigned int);
    /**
    * Returns ANIM_RTTI to identify AnimPieces from other QCanvasSprites.
    * @return Returns ANIM_RTTI.
    */
    int rtti() const;
    /**
    * For checking if the AnimPiece is currently playing its animation.
    * @return Returns TRUE if animation is currently playing. Returns FALSE otherwise.
    */
    bool isPlaying();
    /**
    * Starts playing the AnimPiece animation, delays animation start by seq * FRAME_DELAY ms
    * or, if jitter is TRUE, by a random amount between zero and JITTER * FRAME_DELAY ms.
    * @param p Pointer to piece the AnimPiece flips after the finishing playing its animation.
    * @param seq Sequence number. Used for determining animation start delay. Values under zero get handled as zero.
    * @param jitter Whether to use random animation start delay or not. TRUE for yes, FALSE for no.
    */
    void animPlay(Piece* p, double seq=0, bool jitter=FALSE);
    /**
    * Get animation end result.
    * @return Returns BLACK or WHITE per the direction-property.
    */
    unsigned int getResult();
    /**
    * Stops playing the animation. Stops all timers, resets frame counter, hides the animation, set piece color.
    */
    void stopPlaying();
    
protected slots:
    /**
    * Advances to the next frame, if the current frame is last, hide animation and set piece color. Gets called by Timer.
    */
    void advanceFrame();
    /**
    * Starts playing the animation. Gets called by STimer.
    */
    void startAnim();
        
protected:
    /**
    * Current frame of the animation.
    */
    int currentFrame;
    /**
    * Is BTW if the animation is from black to white, WTB if the animation is from white to black.
    */
    unsigned int direction;
    /**
    * Is TRUE if animation is playing, otherwise FALSE.
    */
    bool playing;
    /**
    * Piece to set color of after animation played.
    */
    Piece* piece;
    /**
    * Animation start delay timer.
    */
    QTimer* STimer;
    /**
    * Animation frame-to-frame delay timer.
    */
    QTimer* Timer;
};


#endif  // ANIMPIECE_H
