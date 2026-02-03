import { BLANK, BLACK, WHITE, BOARDSIZE } from './constants';
import type { TCoord } from './types';
import { TRules } from './rules';

export class GameRules extends TRules {
  private changes: number[] = [];
  private turn: number = BLACK;

  constructor(weird = false) {
    super(weird);
    this.turn = BLACK;
  }

  setPiece(pos: number, color: number = BLANK): boolean {
    if (this.turn !== BLANK) {
      if (color === BLANK) {
        color = this.turn;
      }
      const prevBoard = this.getBoard();
      if (this.rulesSetStone(this.uiToTCoord(pos), color) === 0) {
        this.updateBoard(pos, prevBoard);
        this.updateTurn();
        return true;
      }
      return false;
    }
    return false;
  }

  getPiece(pos: number): number {
    return this.stoneAt(this.uiToTCoord(pos));
  }

  getTurn(): number {
    return this.turn;
  }

  getScore(color: number): number {
    if (color === BLACK) {
      return this.scoreBlack();
    }
    return this.scoreWhite();
  }

  getChanges(): number[] {
    return [...this.changes];
  }

  getBoard(): number[] {
    return [...this.board];
  }

  uiToTCoord(pos: number): TCoord {
    return { x: pos % 8, y: Math.floor(pos / 8) };
  }

  TCoordToUi(pos: TCoord): number {
    return pos.x + pos.y * 8;
  }

  private updateBoard(pos: number, prevBoard): void {
    this.changes = [];
    this.changes.push(this.getTurn());
    for (let i = 0; i < prevBoard.length; i++) {
      if (prevBoard[i] !== this.board[i]) {
        this.changes.push(i);
      }
    }
  }

  private updateTurn(): void {
    const newTurn = this.getTurn() === BLACK ? WHITE : BLACK;

    if (this.movePossible(newTurn) === 0) {
      this.turn = newTurn;
    } else if (this.movePossible(this.turn) === -1) {
      this.turn = BLANK;
    }
  }
}
