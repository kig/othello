import { defineConfig } from 'vite';
import { viteSingleFile } from "vite-plugin-singlefile";

export default defineConfig({
  plugins: [viteSingleFile()],
  root: '.',
  publicDir: 'public',

  // Relative base so the built app works when index.html is served from a subdirectory.
  base: './',
  build: {
    assetsInlineLimit: 100000000,
  },
});
