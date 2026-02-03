import {
  BLANK,
  BLACK,
  WHITE,
  COLUMNS,
  ROWS,
  DEFAULT_BG,
  DEFAULT_BLANK,
  DEFAULT_BLACK,
  DEFAULT_WHITE,
  DEFAULT_BTW,
  DEFAULT_WTB,
  BTW,
  WTB,
} from './constants';
import type { ImageMap } from './types';
import { GameRules } from './gamerules';
import { Piece } from './piece';
import { AnimPiece } from './animpiece';
import { AIPlayer } from './aiplayer';

const BOTH = 999999;

export class BoardView {
  private canvas: HTMLCanvasElement;
  private ctx: CanvasRenderingContext2D;
  private images: ImageMap;
  private rules = new GameRules();
  private pieces: Piece[] = [];
  private wtbPieces: AnimPiece[] = [];
  private btwPieces: AnimPiece[] = [];
  private players: AIPlayer[] = [];
  private animations = true;
  private animSpeed = 1;
  private animRandomJitter = false;
  private animType = 2;
  private player = BLACK;
  private space = 1;
  private statusCallback: (message: string) => void;
  private animationHandle: number | null = null;

  constructor(canvas: HTMLCanvasElement, images: ImageMap, statusCallback: (message: string) => void) {
    this.canvas = canvas;
    const ctx = canvas.getContext('2d');
    if (!ctx) {
      throw new Error('Unable to get 2d context');
    }
    this.ctx = ctx;
    this.images = images;
    this.statusCallback = statusCallback;

    this.canvas.width = 450;
    this.canvas.height = 450;

    for (let i = 0; i < COLUMNS * ROWS; i++) {
      const piece = new Piece();
      piece.setPosition(i);
      piece.setColor(BLANK);
      this.pieces.push(piece);
    }

    for (let i = 0; i < COLUMNS * ROWS; i++) {
      const wtb = new AnimPiece(WTB);
      wtb.setPosition(i);
      this.wtbPieces.push(wtb);
      const btw = new AnimPiece(BTW);
      btw.setPosition(i);
      this.btwPieces.push(btw);
    }

    this.newMove();
    this.newBlackSingleGame();

    this.canvas.addEventListener('click', (event) => this.handleClick(event));
    this.startRenderLoop();
  }

  setAnimated(animate: boolean): void {
    this.animations = animate;
  }

  getAnimated(): boolean {
    return this.animations;
  }

  setAnimSpeed(speed: number): void {
    if (speed >= 0) {
      this.animSpeed = speed;
      this.animRandomJitter = false;
    } else {
      this.animSpeed = 1;
      this.animRandomJitter = true;
    }
  }

  getAnimSpeed(): number {
    return this.animSpeed;
  }

  newBlackSingleGame(): void {
    this.disconnectAllPlayers();
    this.space = 1;

    const ai = new AIPlayer(WHITE);
    this.connectPlayer(ai);
    ai.onMakeMove = (pos) => this.extMove(pos);

    this.newGame(BLACK);
  }

  newWhiteSingleGame(): void {
    this.disconnectAllPlayers();
    this.space = 1;

    const ai = new AIPlayer(BLACK);
    this.connectPlayer(ai);
    ai.onMakeMove = (pos) => this.extMove(pos);

    this.newGame(WHITE);
  }

  newTwoGame(): void {
    this.disconnectAllPlayers();
    this.space = 0;

    this.newGame(BOTH);
  }

  newWeirdTwoGame(): void {
    this.disconnectAllPlayers();
    this.space = 0;

    this.newGame(BOTH, true);
  }

  newDemoGame(): void {
    this.disconnectAllPlayers();
    this.space = 2;

    const aiWhite = new AIPlayer(WHITE);
    this.connectPlayer(aiWhite);
    aiWhite.onMakeMove = (pos) => this.extMove(pos);

    const aiBlack = new AIPlayer(BLACK);
    this.connectPlayer(aiBlack);
    aiBlack.onMakeMove = (pos) => this.extMove(pos);

    this.newGame(BLANK);
  }

  toggleAnim(): void {
    this.setAnimated(!this.animations);
  }

  private updateView(): void {
    if (this.animations) {
      this.animateBoard();
    } else {
      this.clearBoard();
    }
  }

  private animateBoard(): void {
    let i = -1;
    let delay = 0;
    for (const piece of this.pieces) {
      const newFrame = this.rules.getPiece(piece.getPosition());
      if (piece.getColor() !== newFrame) {
        if (newFrame === BLANK || piece.getColor() === BLANK) {
          piece.setColor(newFrame);
          piece.setFrame(newFrame);
        } else {
          piece.setColor(newFrame);
          piece.setFrame(newFrame);
          
          for (const anim of [...this.btwPieces, ...this.wtbPieces]) {
            if (anim.getResult() === newFrame && anim.position === piece.getPosition()) {
              if (!this.animRandomJitter) {
                i = this.animSpeed * this.animDelayAlgo(this.animType, piece, (value) => (delay = value), delay);
              } else {
                i++;
              }
              anim.animPlay(piece, i, this.animRandomJitter);
              break;
            }
          }
          
        }
      }
    }
  }

  private animDelayAlgo(algo: number, piece: Piece, setDelay: (value: number) => void, delay: number): number {
    switch (algo) {
      case 1:
        return 5.5 * Math.log10(3.5 * this.distanceFromNew(piece.getPosition()) - 3.0);
      case 2:
        return this.distanceFromNew(piece.getPosition()) - 1.0;
      case 3:
        return (4 * this.getMaximumDistance()) / (5.5 * Math.log10(3.5 * this.distanceFromNew(piece.getPosition()) - 3.0));
      case 4:
        return (3 * this.getMaximumDistance()) / (1 + Math.abs(this.distanceFromNew(piece.getPosition()))) - 1.0;
      case 5:
        return this.distanceFromNew(piece.getPosition()) - 1.0 + Math.PI + this.angleFromNew(piece.getPosition());
      default:
        const retval = delay;
        setDelay(delay + 1);
        return retval;
    }
  }

  private angleFromNew(pos: number): number {
    const changes = this.rules.getChanges();
    const newpos = changes[1];
    const p = this.rules.uiToTCoord(pos);
    const n = this.rules.uiToTCoord(newpos);

    const diffX = p.x - n.y;
    const diffY = p.y - n.y;

    return Math.atan2(diffX, diffY);
  }

  private getMaximumDistance(): number {
    let maxDistance = 0;
    const changes = this.rules.getChanges();
    for (let idx = 2; idx < changes.length; idx++) {
      const temp = this.distanceFromNew(changes[idx]);
      if (temp > maxDistance) {
        maxDistance = temp;
      }
    }
    return maxDistance;
  }

  private distanceFromNew(pos: number): number {
    const changes = this.rules.getChanges();
    const newpos = changes[1];
    const p = this.rules.uiToTCoord(pos);
    const n = this.rules.uiToTCoord(newpos);

    const absX = Math.abs(p.x - n.x);
    const absY = Math.abs(p.y - n.y);
    return absX < absY ? absY : absX;
  }

  private clearBoard(): void {
    for (const piece of this.pieces) {
      const newFrame = this.rules.getPiece(piece.getPosition());
      piece.setColor(newFrame);
      piece.setFrame(newFrame);
    }
    for (const anim of [...this.btwPieces, ...this.wtbPieces]) {
      anim.stopPlaying();
    }
  }

  private isSpace(): boolean {
    return this.space > 0;
  }

  private connectPlayer(player: AIPlayer): boolean {
    if (this.isSpace()) {
      this.space--;
      this.players.push(player);
      player.onDisconnect = () => this.disconnectPlayer(player);
      return true;
    }
    return false;
  }

  private handleClick(event: MouseEvent): void {
    const rect = this.canvas.getBoundingClientRect();

    // If canvas is CSS-scaled (PWA/mobile), map pointer coords back to internal canvas coords.
    const scaleX = this.canvas.width / rect.width;
    const scaleY = this.canvas.height / rect.height;

    const x = (event.clientX - rect.left) * scaleX;
    const y = (event.clientY - rect.top) * scaleY;

    const col = Math.floor(x / 56);
    const row = Math.floor(y / 56);
    const pos = row * 8 + col;

    const piece = this.pieces[pos];
    if (!piece) {
      return;
    }
    if (piece.getFrame() === BLANK) {
      if (this.player !== BLANK && (this.player === BOTH || this.rules.getTurn() === this.player) && this.makeMove(pos)) {
        // handled
      }
    }
  }

  private extMove(pos: number): void {
    if (this.rules.getTurn() !== this.player || this.player === BLANK) {
      this.makeMove(pos);
    }
  }

  private newGame(newPlayer: number, weird = false): void {
    this.player = newPlayer;
    this.rules = new GameRules(weird);
    this.clearBoard();
    this.newMove();
    if (this.player !== BLACK) {
      this.signalMoveMade(0);
    }
  }

  private disconnectAllPlayers(): void {
    for (const player of this.players) {
      this.disconnectPlayer(player);
    }
    this.players = [];
  }

  private disconnectPlayer(player: AIPlayer): void {
    player.onMakeMove = null;
    player.onDisconnect = null;
  }

  private makeMove(pos: number): boolean {
    const retval = this.rules.setPiece(pos);
    if (retval) {
      this.newMove();
      this.signalMoveMade(pos);
    }
    return retval;
  }

  private signalMoveMade(pos: number): void {
    for (const player of this.players) {
      player.extMove(pos);
    }
  }

  private newMove(): void {
    this.updateView();

    let msg = '';
    if (this.rules.getTurn() !== BLANK) {
      if (this.rules.getTurn() === BLACK) {
        msg = "Black's";
      } else if (this.rules.getTurn() === WHITE) {
        msg = "White's";
      }
      if (this.rules.getTurn() === this.player) {
        msg += ' (your)';
      }
      msg += ' turn.';
    } else {
      const bScore = this.rules.getScore(BLACK);
      const wScore = this.rules.getScore(WHITE);
      msg = 'Game over! ';
      if (bScore > wScore) {
        msg += 'Black wins';
      } else if (bScore < wScore) {
        msg += 'White wins';
      } else {
        msg += 'Tie';
      }
      msg += ` (${bScore} - ${wScore}).`;
    }
    this.statusCallback(msg);
  }

  private startRenderLoop(): void {
    const render = (now: number) => {
      this.render(now);
      this.animationHandle = requestAnimationFrame(render);
    };
    this.animationHandle = requestAnimationFrame(render);
  }

  private render(now: number): void {
    const ctx = this.ctx;
    const bg = this.images[DEFAULT_BG];
    if (bg) {
      ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
      ctx.drawImage(bg, 0, 0, this.canvas.width, this.canvas.height);
    }

    for (const piece of this.pieces) {
      let sprite = this.images[DEFAULT_BLANK];
      if (piece.getFrame() === BLACK) {
        sprite = this.images[DEFAULT_BLACK];
      } else if (piece.getFrame() === WHITE) {
        sprite = this.images[DEFAULT_WHITE];
      }
      const { x, y } = piece.getPixel();
      ctx.drawImage(sprite, x, y, sprite.width, sprite.height);
    }

    for (const anim of [...this.btwPieces, ...this.wtbPieces]) {
      if (anim.playing) {
        const updated = anim.advanceFrame(now);
        const frames = anim.direction === BTW ? DEFAULT_BTW : DEFAULT_WTB;
        const frameIndex = !updated && anim.currentFrame === 0 ? frames.length-1 : Math.min(anim.currentFrame, frames.length - 1);
        const frame = this.images[frames[frameIndex]];
        const { x, y } = anim.getPixel();
        ctx.drawImage(frame, x, y, frame.width, frame.height);
      }
    }
  }
}
