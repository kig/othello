import {
  COLUMNS,
  ROWS,
  SQUARE_W,
  SQUARE_H,
  FRAME_DELAY,
  JITTER,
  WTB,
  BTW,
  WHITE,
  BLACK,
  FRAMES,
  WTB_X_OFFSET,
  WTB_Y_OFFSET,
  BTW_X_OFFSET,
  BTW_Y_OFFSET,
} from './constants';
import type { Piece } from './piece';

export class AnimPiece {
  position = ROWS * COLUMNS;
  currentFrame = 0;
  direction = BTW;
  playing = false;
  piece: Piece | null = null;
  frameAt = 0;
  startAt = 0;

  constructor(direction: number) {
    this.direction = direction;
  }

  setPosition(pos: number): boolean {
    if (pos < 64) {
      this.position = pos;
      return true;
    }
    return false;
  }

  getResult(): number {
    return this.direction === BTW ? WHITE : BLACK;
  }

  animPlay(piece: Piece, seq = 0, jitter = false): void {
    this.playing = true;
    this.piece = piece;
    this.setPosition(piece.getPosition());
    piece.setColor(this.getResult());

    let startDelay = 0;
    if (jitter && seq > 0) {
      startDelay = Math.floor(Math.random() * JITTER * FRAME_DELAY);
    } else {
      startDelay = Math.floor(FRAME_DELAY * seq);
    }

    this.currentFrame = 0;
    this.startAt = performance.now() + startDelay;
    this.frameAt = this.startAt;
  }

  stopPlaying(): void {
    this.playing = false;
    this.currentFrame = 0;
    this.piece = null;
  }

  advanceFrame(now: number): boolean {
    if (!this.playing || now < this.startAt) {
      return false;
    }

    if (now >= this.frameAt + FRAME_DELAY) {
      if (this.currentFrame < FRAMES) {
        this.currentFrame += 1;
        this.frameAt = now;
        return true;
      }
      if (this.piece) {
        this.piece.setColor(this.getResult());
      }
      this.stopPlaying();
    }
    return false;
  }

  getPixel(): { x: number; y: number } {
    const x = this.position % COLUMNS;
    const y = Math.floor(this.position / ROWS);
    const xOffset = this.direction === WTB ? WTB_X_OFFSET : BTW_X_OFFSET;
    const yOffset = this.direction === WTB ? WTB_Y_OFFSET : BTW_Y_OFFSET;
    return { x: x * SQUARE_W + xOffset, y: y * SQUARE_H + yOffset };
  }
}
