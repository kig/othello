import { describe, expect, it } from 'vitest';
import { BLANK, BLACK, WHITE } from './constants';
import { GameRules } from './gamerules';

describe('GameRules', () => {
  it('starts with initial four pieces and black to move', () => {
    const rules = new GameRules(false);

    expect(rules.getTurn()).toBe(BLACK);

    // Initial position per original C++ board.cpp
    expect(rules.getPiece(3 + 3 * 8)).toBe(BLACK);
    expect(rules.getPiece(4 + 4 * 8)).toBe(BLACK);
    expect(rules.getPiece(3 + 4 * 8)).toBe(WHITE);
    expect(rules.getPiece(4 + 3 * 8)).toBe(WHITE);

    // A random empty square
    expect(rules.getPiece(0)).toBe(BLANK);
  });

  it('rejects illegal move into occupied square', () => {
    const rules = new GameRules(false);
    expect(rules.setPiece(3 + 3 * 8)).toBe(false);
  });
});
