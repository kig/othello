/***************************************************************************
                          main.cpp  -  description
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

#include <qapplication.h>

#include "othello.h"


/**
* Othello main program. Sets About-data and processes command line arguments.
* Creates a new Othello game window and makes it non-resizable.
*/
int main(int argc, char *argv[])
{
  QApplication a( argc, argv );
  Othello *othello = new Othello();
  a.setMainWidget(othello);
  othello->show();
  othello->setMaximumSize(othello->size());
  othello->setMinimumSize(othello->size());

  return a.exec();
}
