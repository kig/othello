import {
  DEFAULT_BG,
  DEFAULT_BLANK,
  DEFAULT_BLACK,
  DEFAULT_WHITE,
  DEFAULT_WTB,
  DEFAULT_BTW,
} from './constants';
import type { ImageMap } from './types';

const assetPaths = [DEFAULT_BG, DEFAULT_BLANK, DEFAULT_BLACK, DEFAULT_WHITE, ...DEFAULT_WTB, ...DEFAULT_BTW];

export async function loadImages(): Promise<ImageMap> {
  const images: ImageMap = {};
  await Promise.all(
    assetPaths.map(
      (src) =>
        new Promise<void>((resolve, reject) => {
          const img = new Image();
          img.onload = () => {
            images[src] = img;
            resolve();
          };
          img.onerror = () => reject(new Error(`Failed to load image ${src}`));
          img.src = src;
        }),
    ),
  );
  return images;
}
