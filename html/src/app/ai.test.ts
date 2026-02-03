import { describe, expect, it } from 'vitest';
import { BLACK } from './constants';
import { TRules } from './rules';
import { findMove } from './ai';

describe('AI', () => {
  it('does not crash on initial board', () => {
    const rules = new TRules(false);
    const move = findMove(rules, BLACK);
    expect(move).toHaveProperty('x');
    expect(move).toHaveProperty('y');
  });
});
