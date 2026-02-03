import { BLANK, BLACK, WHITE } from './constants';
import { GameRules } from './gamerules';
import { TRules } from './rules';
import { findMove } from './ai';

export class AIPlayer {
  private rules = new GameRules();
  private trules = new TRules();
  private color: number;
  private opposite: number;
  private lastPos = 0;
  private timeoutId: number | null = null;

  onMakeMove: ((pos: number) => void) | null = null;
  onDisconnect: (() => void) | null = null;

  constructor(color: number) {
    this.color = color;
    this.opposite = color === BLACK ? WHITE : BLACK;
  }

  extMove(pos: number): void {
    this.lastPos = pos;
    if (this.timeoutId !== null) {
      window.clearTimeout(this.timeoutId);
    }
    this.timeoutId = window.setTimeout(() => this.processMove(), 600);
  }

  private processMove(): void {
    if (this.rules.getTurn() === this.opposite) {
      if (this.rules.setPiece(this.lastPos, this.opposite)) {
        this.trules.rulesSetStone(this.rules.uiToTCoord(this.lastPos), this.opposite);
      }
    }
    if (this.rules.getTurn() === this.color) {
      const tmove = findMove(this.trules, this.color);
      const move = this.rules.TCoordToUi(tmove);
      if (this.rules.setPiece(move, this.color)) {
        this.trules.rulesSetStone(tmove, this.color);
        this.onMakeMove?.(move);
      }
    }
    if (this.rules.getTurn() === BLANK) {
      this.onDisconnect?.();
    }
  }
}
