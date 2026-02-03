import { BLACK, WHITE, BOARDSIZE, BLANK } from './constants';
import type { TCoord } from './types';
import type { TRules } from './rules';

const STEPS = 2;

export function findMove(othello: TRules, player: number): TCoord {
  let moveNumber = 0;
  const moveList = possibleMoves(othello, player, (count) => (moveNumber = count));
  sortScores(othello, moveList, moveNumber, player);

  if (moveList.length === 0) {
    // Shouldn't normally happen when called correctly, but avoid crashing.
    return { x: 0, y: 0 };
  }

  let nextMove = moveList[0];
  let previousScore = tryNext(othello, player, nextMove, STEPS);

  for (let k = 1; k < Math.floor(moveNumber / 3); k++) {
    const newScore = tryNext(othello, player, moveList[k], STEPS);
    if (newScore > previousScore) {
      nextMove = moveList[k];
      previousScore = newScore;
    } else if (newScore === previousScore && Math.random() > 0.5) {
      nextMove = moveList[k];
      previousScore = newScore;
    }
  }

  return nextMove;
}

function tryNext(othello: TRules, player: number, place: TCoord, steps: number): number {
  let score = 0;
  let tempScore = 0;
  let moveNumber = 0;
  const enemy = player === WHITE ? BLACK : WHITE;

  score = moveScore(othello, player, place);
  const nextBoard = othello.clone();
  nextBoard.rulesSetStone(place, player);

  if (steps > 0) {
    const enemyMoves = possibleMoves(nextBoard, enemy, (count) => (moveNumber = count));
    sortScores(nextBoard, enemyMoves, moveNumber, enemy);

    for (let i = 0; i < Math.floor(moveNumber / 5); i++) {
      tempScore = nextEnemyMove(nextBoard, player, enemyMoves[i], steps - 1);
      if (tempScore > score) {
        score = tempScore;
      }
    }
  }

  return score;
}

function nextEnemyMove(othello: TRules, player: number, place: TCoord, steps: number): number {
  let moveNumber = 0;
  let score = 0;
  let tempScore = 0;
  const enemy = player === WHITE ? BLACK : WHITE;

  const nextBoard = othello.clone();
  nextBoard.rulesSetStone(place, enemy);

  const moveList = possibleMoves(nextBoard, player, (count) => (moveNumber = count));
  sortScores(nextBoard, moveList, moveNumber, player);

  for (let i = 0; i < Math.floor(moveNumber / 4); i++) {
    if (i === 0) {
      score = tryNext(nextBoard, player, moveList[0], steps);
    } else {
      tempScore = tryNext(nextBoard, player, moveList[i], steps);
      if (tempScore > score) {
        score = tempScore;
      }
    }
  }

  return score;
}

function moveScore(othello: TRules, player: number, place: TCoord): number {
  let score = 0;
  let newMoves = 0;
  const enemy = player === WHITE ? BLACK : WHITE;

  score += evaluateLocation(othello, place, player);
  const nextBoard = othello.clone();
  nextBoard.rulesSetStone(place, player);

  score -= countLiberties(nextBoard, player);

  possibleMoves(nextBoard, player, (count) => (newMoves = count));
  score += newMoves;

  possibleMoves(nextBoard, enemy, (count) => (newMoves = count));
  score -= newMoves;

  return score;
}

function turnedStones(othello: TRules, player: number, place: TCoord): number {
  let score = 0;
  let enemyCount = 0;
  let check: TCoord = { x: 0, y: 0 };
  const enemy = player === WHITE ? BLACK : WHITE;

  for (let k = 1; k <= 8; k++) {
    const direction = getDirection(k);
    check = { x: place.x + direction.x, y: place.y + direction.y };

    if (othello.stoneAt(check) === enemy) {
      enemyCount = 0;
      do {
        enemyCount++;
        check = { x: check.x + direction.x, y: check.y + direction.y };
      } while (othello.stoneAt(check) === enemy);

      if (othello.stoneAt(check) === BLANK) {
        score += enemyCount;
      }
    }
  }

  return score;
}

function countLiberties(othello: TRules, player: number): number {
  let liberties = 0;

  for (let i = 0; i < BOARDSIZE; i++) {
    for (let j = 0; j < BOARDSIZE; j++) {
      if (othello.getStoneAt(i, j) === BLANK) {
        let foundOwn = 0;

        for (let k = 1; k <= 8 && foundOwn === 0; k++) {
          const direction = getDirection(k);
          if (othello.getStoneAt(i + direction.x, j + direction.y) === player) {
            foundOwn = 1;
          }
        }

        if (foundOwn === 1) {
          liberties++;
        }
      }
    }
  }

  return liberties;
}

function evaluateLocation(othello: TRules, place: TCoord, player: number): number {
  const enemy = player === WHITE ? BLACK : WHITE;

  if (place.y === 0) {
    let i = 0;
    for (i = 0; i < place.x && othello.getStoneAt(i, 0) === player; i++);
    if (i === place.x) {
      return 40;
    }
    for (i = BOARDSIZE - 1; i > place.x && othello.getStoneAt(i, 0) === player; i--);
    if (i === place.x) {
      return 40;
    }
    for (i = 0; i < place.x && othello.getStoneAt(i, 0) === enemy; i++);
    if (i === place.x && othello.getStoneAt(i - 1, 0) === enemy) {
      return -20;
    }
    for (i = BOARDSIZE - 1; i > place.x && othello.getStoneAt(i, 0) === enemy; i--);
    if (i === place.x && othello.getStoneAt(i + 1, 0) === enemy) {
      return -20;
    }
    return 20;
  }

  if (place.y === BOARDSIZE - 1) {
    let i = 0;
    for (i = 0; i < place.x && othello.getStoneAt(i, BOARDSIZE - 1) === player; i++);
    if (i === place.x) {
      return 40;
    }
    for (i = BOARDSIZE - 1; i > place.x && othello.getStoneAt(i, BOARDSIZE - 1) === player; i--);
    if (i === place.x) {
      return 40;
    }
    for (i = 0; i < place.x && othello.getStoneAt(i, BOARDSIZE - 1) === enemy; i++);
    if (i === place.x && othello.getStoneAt(i - 1, BOARDSIZE - 1) === enemy) {
      return -20;
    }
    for (i = BOARDSIZE; i > place.x && othello.getStoneAt(i, BOARDSIZE - 1) === enemy; i--);
    if (i === place.x && othello.getStoneAt(i + 1, BOARDSIZE - 1) === enemy) {
      return -20;
    }
    return 20;
  }

  if (place.x === 0) {
    let i = 0;
    for (i = 0; i < place.y && othello.getStoneAt(0, i) === player; i++);
    if (i === place.y) {
      return 40;
    }
    for (i = BOARDSIZE - 1; i > place.y && othello.getStoneAt(0, i) === player; i--);
    if (i === place.y) {
      return 40;
    }
    for (i = 0; i < place.y && othello.getStoneAt(0, i) === enemy; i++);
    if (i === place.y && othello.getStoneAt(0, i - 1) === enemy) {
      return -20;
    }
    for (i = BOARDSIZE - 1; i > place.y && othello.getStoneAt(0, i) === enemy; i--);
    if (i === place.y && othello.getStoneAt(0, i + 1) === enemy) {
      return -20;
    }
    return 20;
  }

  if (place.x === BOARDSIZE - 1) {
    let i = 0;
    for (i = 0; i < place.y && othello.getStoneAt(BOARDSIZE - 1, i) === player; i++);
    if (i === place.y) {
      return 40;
    }
    for (i = BOARDSIZE - 1; i > place.y && othello.getStoneAt(BOARDSIZE - 1, i) === player; i--);
    if (i === place.y) {
      return 40;
    }
    for (i = 0; i < place.y && othello.getStoneAt(BOARDSIZE - 1, i) === enemy; i++);
    if (i === place.y && othello.getStoneAt(BOARDSIZE - 1, i - 1) === enemy) {
      return -20;
    }
    for (i = BOARDSIZE - 1; i > place.y && othello.getStoneAt(BOARDSIZE - 1, i) === enemy; i--);
    if (i === place.y && othello.getStoneAt(BOARDSIZE - 1, i + 1) === enemy) {
      return -20;
    }
    return 20;
  }

  return 0;
}

function possibleMoves(othello: TRules, player: number, setCount: (count: number) => void): TCoord[] {
  const moveList: TCoord[] = [];
  let moveNumber = 0;
  const enemy = player === WHITE ? BLACK : WHITE;

  for (let x = 0; x < BOARDSIZE; x++) {
    for (let y = 0; y < BOARDSIZE; y++) {
      if (othello.getStoneAt(x, y) === player) {
        for (let k = 1; k <= 8; k++) {
          const direction = getDirection(k);
          let check: TCoord = { x: x + direction.x, y: y + direction.y };

          if (othello.stoneAt(check) === enemy) {
            do {
              check = { x: check.x + direction.x, y: check.y + direction.y };
            } while (othello.stoneAt(check) === enemy);

            if (
              othello.stoneAt(check) === BLANK &&
              check.x >= 0 &&
              check.x < BOARDSIZE &&
              check.y >= 0 &&
              check.y < BOARDSIZE
            ) {
              moveNumber++;
              moveList.push(check);
            }
          }
        }
      }
    }
  }

  setCount(moveNumber);
  return moveList;
}

function getDirection(k: number): TCoord {
  const direction: TCoord = { x: 0, y: 0 };

  switch (k) {
    case 1:
      direction.x = 1;
      direction.y = 0;
      break;
    case 2:
      direction.x = 1;
      direction.y = 1;
      break;
    case 3:
      direction.x = 0;
      direction.y = 1;
      break;
    case 4:
      direction.x = -1;
      direction.y = 1;
      break;
    case 5:
      direction.x = -1;
      direction.y = 0;
      break;
    case 6:
      direction.x = -1;
      direction.y = -1;
      break;
    case 7:
      direction.x = 0;
      direction.y = -1;
      break;
    case 8:
      direction.x = 1;
      direction.y = -1;
      break;
  }

  return direction;
}

function sortScores(othello: TRules, moves: TCoord[], moveNum: number, player: number): void {
  for (let i = 1; i < moveNum; i++) {
    const temp: TCoord = { x: moves[i].x, y: moves[i].y };
    let j = i;

    while (j > 0 && moveScore(othello, player, moves[j - 1]) < moveScore(othello, player, temp)) {
      moves[j] = { x: moves[j - 1].x, y: moves[j - 1].y };
      j--;
    }

    moves[j] = { x: temp.x, y: temp.y };
  }
}

export const _aiInternals = {
  turnedStones,
  countLiberties,
  possibleMoves,
  getDirection,
};
