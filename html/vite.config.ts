import { defineConfig } from 'vite';

export default defineConfig({
  root: '.',
  publicDir: 'public',

  // Relative base so the built app works when index.html is served from a subdirectory.
  base: './',
});
