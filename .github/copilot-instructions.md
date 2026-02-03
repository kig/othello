# Copilot instructions for this repository

## Project overview
- This repo contains the original Qt/C++ Othello implementation plus a 1:1 HTML/TypeScript port in `html/`.
- The web port is built with Vite and mirrors the original rules, AI, assets, and animation timing.

## Where to work
- **Web app**: `html/` (Vite, TypeScript). Entry: `html/src/main.ts`, app: `html/src/app/`.
- **Assets**: `html/public/pics` and `html/public/icons` (copied from root `pics/` and `icons/`).
- **Original C++**: root `.cpp/.h` files (reference only, do not change unless requested).

## Build & run
- `cd html`
- `npm install`
- `npm run dev` (local dev server)
- `npm run build` (production build)

## Porting expectations
- Keep logic and constants line-by-line compatible with the C++ sources (`board.*`, `rules.*`, `gamerules.*`, `ai.*`, `boardview.*`).
- Preserve animation timing, offsets, and sprite usage (do not resize or replace images).
- Prefer small, surgical edits; avoid refactors that diverge from the original behavior.

## Coding style
- Use strict TypeScript, no `any` or unsafe casts.
- Avoid new dependencies unless required for parity or build.
- Match existing module layout and naming conventions under `html/src/app/`.

## Testing & verification
- No automated tests are present; validate manually by running the Vite app and playing games in each mode.
