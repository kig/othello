import { BLANK, BLACK, WHITE, BOARDSIZE } from './constants';
import type { TCoord } from './types';

export class TBoard {
  protected board: number[] = [];

  constructor() {
    this.init();
  }

  init(): void {
    this.board = new Array(BOARDSIZE*BOARDSIZE).fill(BLANK);
    this.setStone({x: 3, y: 3}, BLACK);
    this.setStone({x: 4, y: 4}, BLACK);
    this.setStone({x: 3, y: 4}, WHITE);
    this.setStone({x: 4, y: 3}, WHITE);
  }

  stoneAt(location: TCoord): number {
    if (location.x < 0 || location.x >= BOARDSIZE || location.y < 0 || location.y >= BOARDSIZE) {
      return BLANK;
    }

    if (this.board[location.y*BOARDSIZE+location.x] === WHITE) {
      return WHITE;
    }
    if (this.board[location.y*BOARDSIZE+location.x] === BLACK) {
      return BLACK;
    }
    return BLANK;
  }

  getStoneAt(x: number, y: number): number {
    const temp: TCoord = { x, y };
    return this.stoneAt(temp);
  }

  setStone(location: TCoord, newStone: number): number {
    if (newStone !== BLACK && newStone !== WHITE) {
      return -1;
    }

    if (this.stoneAt(location) === BLANK) {
      this.board[location.y*BOARDSIZE+location.x] = newStone;
      return 0;
    }

    return -1;
  }

  flipStone(location: TCoord): number {
    if (this.board[location.y*BOARDSIZE+location.x] === WHITE) {
      this.board[location.y*BOARDSIZE+location.x] = BLACK;
    } else {
      if (this.board[location.y*BOARDSIZE+location.x] === BLACK) {
        this.board[location.y*BOARDSIZE+location.x] = WHITE;
      } else {
        return -1;
      }
    }
    return 0;
  }
}
