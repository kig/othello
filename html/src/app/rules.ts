import { BLANK, BLACK, WHITE, BOARDSIZE } from './constants';
import type { TCoord } from './types';
import { TBoard } from './board';

export class TRules extends TBoard {
  private weird: boolean;

  constructor(weird = false) {
    super();
    this.weird = weird;
    this.init();
  }

  clone(): TRules {
    const copy = new TRules(this.weird);
    copy.board = [...this.board];
    return copy;
  }

  rulesSetStone(location: TCoord, newStone: number): number {
    if (this.stoneAt(location) !== BLANK) {
      return -1;
    }

    if (this.turnStones(location, newStone) === -1) {
      return -1;
    }

    this.setStone(location, newStone);
    return 0;
  }

  turnStones(location: TCoord, newStone: number): number {
    return this.flipAllDirections(location, newStone, 1);
  }

  movePossible(next: number): number {
    let k = 0;
    const location: TCoord = { x: 0, y: 0 };

    for (let i = 0; i < BOARDSIZE; i++) {
      for (let j = 0; j < BOARDSIZE; j++) {
        if (this.getStoneAt(i, j) === BLANK) {
          location.x = i;
          location.y = j;

          k = this.flipAllDirections(location, next, 0);
          if (k === 0) {
            return 0;
          }
        }
      }
    }

    return -1;
  }

  scoreWhite(): number {
    let score = 0;
    for (let i = 0; i < BOARDSIZE; i++) {
      for (let j = 0; j < BOARDSIZE; j++) {
        if (this.getStoneAt(i, j) === WHITE) {
          score++;
        }
      }
    }
    return score;
  }

  scoreBlack(): number {
    let score = 0;
    for (let i = 0; i < BOARDSIZE; i++) {
      for (let j = 0; j < BOARDSIZE; j++) {
        if (this.getStoneAt(i, j) === BLACK) {
          score++;
        }
      }
    }
    return score;
  }

  private flipAllDirections(location: TCoord, newStone: number, flip = 1): number {
    let retval = 0;
    for (let i = 0; i < 8; i++) {
      retval += this.flipDir(location, i, newStone, flip);
    }
    retval = retval > -8 ? 0 : -1;
    return retval;
  }

  private flipDir(location: TCoord, dir: number, newStone: number, flip = 1): number {
    let next = this.getNextLocation(location, dir);
    let nextStone = this.stoneAt(next);

    if (!this.weird) {
      while (nextStone !== newStone && nextStone !== BLANK) {
        next = this.getNextLocation(next, dir);
        nextStone = this.stoneAt(next);
      }
      if (nextStone === BLANK) {
        return -1;
      }
    }

    next = this.getNextLocation(location, dir);
    nextStone = this.stoneAt(next);

    if (nextStone !== newStone && nextStone !== BLANK) {
      this.flipDir(next, dir, newStone, flip);
      if (flip === 1) {
        this.flipStone(next);
      }
      return 0;
    }
    return -1;
  }

  private getNextLocation(location: TCoord, dir: number): TCoord {
    let x = location.x;
    let y = location.y;
    switch (dir) {
      case 0:
        y++;
        break;
      case 1:
        x++;
        y++;
        break;
      case 2:
        x++;
        break;
      case 3:
        x++;
        y--;
        break;
      case 4:
        y--;
        break;
      case 5:
        x--;
        y--;
        break;
      case 6:
        x--;
        break;
      case 7:
        x--;
        y++;
        break;
    }
    return { x, y };
  }
}
