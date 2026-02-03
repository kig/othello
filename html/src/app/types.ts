export type TCoord = {
  x: number;
  y: number;
};

export type ImageMap = Record<string, HTMLImageElement>;

export type MenuItem = {
  id: string;
  label: string;
  action?: () => void;
  separator?: boolean;
  checked?: () => boolean;
};
