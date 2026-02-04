import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const DIST_DIR = path.resolve(__dirname, '../dist');
const SW_PATH = path.join(DIST_DIR, 'sw.js');

function getFiles(dir, baseDir = '') {
  let results = [];
  if (!fs.existsSync(dir)) return results;
  
  const list = fs.readdirSync(dir);
  list.forEach(file => {
    const fullPath = path.join(dir, file);
    const stat = fs.statSync(fullPath);
    if (stat && stat.isDirectory()) {
      results = results.concat(getFiles(fullPath, path.join(baseDir, file)));
    } else {
      // Filter for js, css, wasm, etc.
      if ((file.endsWith('.js') || file.endsWith('.css') || file.endsWith('.wasm')) && file !== 'sw.js') {
        results.push('./' + path.join(baseDir, file));
      }
    }
  });
  return results;
}

function updateServiceWorker() {
  if (!fs.existsSync(SW_PATH)) {
    console.error('Service Worker file not found at:', SW_PATH);
    process.exit(1);
  }

  const assets = getFiles(DIST_DIR);
  console.log('Found assets to cache:', assets);

  let swContent = fs.readFileSync(SW_PATH, 'utf-8');

  const timestamp = Date.now();
  const newVersion = `othello-pwa-${timestamp}`;

  // Replace version
  // We expect the original line from public/sw.js to be present
  const oldVersionLine = "const CACHE_NAME = 'othello-pwa-v1'; /* BUILD_VERSION_MARKER */";
  const newVersionLine = `const CACHE_NAME = '${newVersion}'; /* BUILD_VERSION_MARKER */`;
  
  if (swContent.includes(oldVersionLine)) {
    swContent = swContent.replace(oldVersionLine, newVersionLine);
  } else {
    console.warn('Could not find version marker line to replace.');
  }

  // Inject assets
  const assetsString = assets.map(a => `'${a}'`).join(',\n  ');
  const assetMarker = "/* BUILD_ASSETS_MARKER */";
  
  if (swContent.includes(assetMarker)) {
     // We prepend a comma because it's inside an array
     swContent = swContent.replace(assetMarker, assets.length > 0 ? `,\n  ${assetsString}` : '');
  } else {
     console.warn('Could not find assets marker to replace.');
  }

  fs.writeFileSync(SW_PATH, swContent);
  console.log(`Service Worker updated with version ${newVersion} and ${assets.length} assets.`);
}

updateServiceWorker();
