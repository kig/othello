import { BLANK, BLACK, WHITE, COLUMNS, ROWS, SQUARE_W, SQUARE_H } from './constants';

export class Piece {
  position = 0;
  color = BLANK;
  frame = BLANK;

  constructor() {
    this.setPosition(0);
    this.setColor(BLANK);
    this.setFrame(BLANK);
  }

  setPosition(newPosition: number): boolean {
    if (newPosition < COLUMNS * ROWS) {
      this.position = newPosition;
      return true;
    }
    return false;
  }

  getPosition(): number {
    return this.position;
  }

  setColor(newColor: number): void {
    if (newColor === BLACK) {
      this.color = BLACK;
    } else if (newColor === WHITE) {
      this.color = WHITE;
    } else {
      this.color = BLANK;
    }
  }

  getColor(): number {
    return this.color;
  }

  setFrame(newFrame: number): void {
    if (newFrame === BLACK) {
      this.frame = BLACK;
    } else if (newFrame === WHITE) {
      this.frame = WHITE;
    } else {
      this.frame = BLANK;
    }
  }

  getFrame(): number {
    return this.frame;
  }

  getPixel(): { x: number; y: number } {
    const x = this.position % COLUMNS;
    const y = Math.floor(this.position / ROWS);
    return { x: x * SQUARE_W + 1, y: y * SQUARE_H + 1 };
  }
}
