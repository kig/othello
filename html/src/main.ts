import { createApp } from './app/app';
import './style.css';

if ('serviceWorker' in navigator) {
  window.addEventListener('load', () => {
    navigator.serviceWorker.register('/sw.js').catch(() => {
      // Best-effort; app works fine without SW.
    });
  });
}

createApp(document.getElementById('app') as HTMLElement);
