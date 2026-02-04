import boardImg from '../pics/board.webp';
import blankImg from '../pics/blank.webp';
import blackImg from '../pics/black.webp';
import whiteImg from '../pics/white.webp';

import wtb1 from '../pics/wtb/white_to_black1.webp';
import wtb2 from '../pics/wtb/white_to_black2.webp';
import wtb3 from '../pics/wtb/white_to_black3.webp';
import wtb4 from '../pics/wtb/white_to_black4.webp';
import wtb5 from '../pics/wtb/white_to_black5.webp';
import wtb6 from '../pics/wtb/white_to_black6.webp';
import wtb7 from '../pics/wtb/white_to_black7.webp';

import btw1 from '../pics/btw/black_to_white1.webp';
import btw2 from '../pics/btw/black_to_white2.webp';
import btw3 from '../pics/btw/black_to_white3.webp';
import btw4 from '../pics/btw/black_to_white4.webp';
import btw5 from '../pics/btw/black_to_white5.webp';
import btw6 from '../pics/btw/black_to_white6.webp';
import btw7 from '../pics/btw/black_to_white7.webp';

export const BOARDSIZE = 8;
export const ROWS = 8;
export const COLUMNS = 8;

export const BLANK = 0;
export const BLACK = 1;
export const WHITE = 2;

export const SQUARE_W = 56;
export const SQUARE_H = 56;

export const FRAME_DELAY = 40;
export const JITTER = 6;
export const FRAMES = 7;

export const WTB = 1;
export const BTW = 0;

export const WTB_X_OFFSET = -51;
export const WTB_Y_OFFSET = -51;
export const BTW_X_OFFSET = -51;
export const BTW_Y_OFFSET = -51;

export const DEFAULT_BG = boardImg;
export const DEFAULT_BLANK = blankImg;
export const DEFAULT_BLACK = blackImg;
export const DEFAULT_WHITE = whiteImg;

export const DEFAULT_WTB = [
  wtb1,
  wtb2,
  wtb3,
  wtb4,
  wtb5,
  wtb6,
  wtb7,
];

export const DEFAULT_BTW = [
  btw1,
  btw2,
  btw3,
  btw4,
  btw5,
  btw6,
  btw7,
];