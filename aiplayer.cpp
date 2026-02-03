/***************************************************************************
             aiplayer.cpp  -  computer player class for othello
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

#include <qtimer.h>

#include "aiplayer.h"
#include "gamerules.h"
#include "ai.h"

AIPlayer::AIPlayer(unsigned int color) : QObject(),
rules(new GameRules()), trules(new TRules()), gTimer(new QTimer()), color(color)
{
    opposite = (color==BLACK) ? WHITE : BLACK;
    connect( gTimer, SIGNAL(timeout()), this, SLOT(processMove()) );
}

AIPlayer::~AIPlayer() {
    gTimer->stop();
    
    delete rules;
    delete trules;
    delete gTimer;
}

void AIPlayer::extMove(unsigned int pos) {
      last_pos = pos;
      gTimer->start( 600, TRUE );
}

void AIPlayer::processMove() {
    unsigned int move;
    TCoord tmove;
    if (rules->getTurn() == opposite) {
        if (rules->setPiece(last_pos,opposite))
            trules->rulesSetStone(rules->uiToTCoord(last_pos),opposite);
    }
    if (rules->getTurn() == color) {
      tmove = findMove(*trules, color);
      move = rules->TCoordToUi(tmove);
      if (rules->setPiece(move, color)) {
        trules->rulesSetStone(tmove, color);
        emit makeMove( move );
      }
    }
    if (rules->getTurn() == BLANK) {
      emit disconnectMe( this );
    }
}
