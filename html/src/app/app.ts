import { loadImages } from './assets';
import { BoardView } from './boardview';

export function createApp(root: HTMLElement): void {
  root.innerHTML = '';

  const menuBar = document.createElement('div');
  menuBar.className = 'menu-bar';

  const leftGroup = document.createElement('div');
  leftGroup.className = 'menu-group';

  const rightGroup = document.createElement('div');
  rightGroup.className = 'menu-group';

  const gameButton = document.createElement('button');
  gameButton.textContent = 'Game';
  gameButton.setAttribute('aria-haspopup', 'true');
  gameButton.setAttribute('aria-expanded', 'false');

  const optionsButton = document.createElement('button');
  optionsButton.textContent = 'Options';
  optionsButton.setAttribute('aria-haspopup', 'true');
  optionsButton.setAttribute('aria-expanded', 'false');

  const helpButton = document.createElement('button');
  helpButton.textContent = 'Help';
  helpButton.setAttribute('aria-haspopup', 'true');
  helpButton.setAttribute('aria-expanded', 'false');

  leftGroup.append(gameButton, optionsButton, helpButton);
  menuBar.append(leftGroup, rightGroup);

  const boardContainer = document.createElement('div');
  boardContainer.className = 'board-container';

  const canvas = document.createElement('canvas');
  canvas.setAttribute('role', 'img');
  canvas.setAttribute('aria-label', 'Othello Game Board');
  boardContainer.appendChild(canvas);

  const statusBar = document.createElement('div');
  statusBar.className = 'status-bar';
  statusBar.textContent = 'Loading...';

  root.append(menuBar, boardContainer, statusBar);

  const aboutDialog = createAboutDialog();
  root.appendChild(aboutDialog);

  let boardView: BoardView | null = null;

  const closeMenus = () => {
    document.querySelectorAll('.menu-panel').forEach((panel) => panel.remove());
    document.querySelectorAll('.menu-group button').forEach((btn) => {
      btn.classList.remove('active');
      btn.setAttribute('aria-expanded', 'false');
    });
  };

  const openMenu = (button: HTMLButtonElement, items: Array<HTMLElement | null>) => {
    closeMenus();
    const panel = document.createElement('div');
    panel.className = 'menu-panel';
    panel.setAttribute('role', 'menu');
    const rect = button.getBoundingClientRect();
    panel.style.top = `${rect.bottom + window.scrollY}px`;
    panel.style.left = `${rect.left + window.scrollX}px`;
    items.forEach((item) => {
      if (item) {
        panel.appendChild(item);
      }
    });
    document.body.appendChild(panel);
    button.classList.add('active');
    button.setAttribute('aria-expanded', 'true');
  };

  const createMenuItem = (label: string, action: () => void) => {
    const item = document.createElement('button');
    item.className = 'menu-item';
    item.setAttribute('role', 'menuitem');
    item.textContent = label;
    item.addEventListener('click', () => {
      action();
      closeMenus();
    });
    return item;
  };

  const createCheckboxItem = (label: string, checked: () => boolean, action: () => void) => {
    const item = document.createElement('button');
    item.className = 'menu-item';
    item.setAttribute('role', 'menuitemcheckbox');
    const isChecked = checked();
    item.setAttribute('aria-checked', isChecked ? 'true' : 'false');
    item.textContent = (isChecked ? '✓ ' : '   ') + label;
    item.addEventListener('click', () => {
      action();
      closeMenus();
    });
    return item;
  };

  const createSeparator = () => {
    const separator = document.createElement('div');
    separator.className = 'menu-separator';
    return separator;
  };

  gameButton.addEventListener('click', () => {
    if (!boardView) {
      return;
    }
    openMenu(gameButton, [
      createMenuItem('Black Vs. Computer', () => boardView?.newBlackSingleGame()),
      createMenuItem('White Vs. Computer', () => boardView?.newWhiteSingleGame()),
      createSeparator(),
      createMenuItem('Two Player Game', () => boardView?.newTwoGame()),
      createMenuItem('Weird Two Player Game', () => boardView?.newWeirdTwoGame()),
      createSeparator(),
      createMenuItem('View Demo Game', () => boardView?.newDemoGame()),
    ]);
  });

  optionsButton.addEventListener('click', () => {
    if (!boardView) {
      return;
    }
    openMenu(optionsButton, [
      createCheckboxItem('Animated flips', () => boardView?.getAnimated() ?? true, () => boardView?.toggleAnim()),
      createSeparator(),
      createMenuItem('Animation speed: Fastest', () => boardView?.setAnimSpeed(0)),
      createMenuItem('Animation speed: Fast', () => boardView?.setAnimSpeed(0.5)),
      createMenuItem('Animation speed: Normal', () => boardView?.setAnimSpeed(1)),
      createMenuItem('Animation speed: Slow', () => boardView?.setAnimSpeed(2)),
      createMenuItem('Animation speed: Slowest', () => boardView?.setAnimSpeed(3)),
      createSeparator(),
      createMenuItem('Animation speed: Random', () => boardView?.setAnimSpeed(-1)),
    ]);
  });

  helpButton.addEventListener('click', () => {
    openMenu(helpButton, [createMenuItem('About', () => showAbout(aboutDialog))]);
  });

  document.addEventListener('click', (event) => {
    const target = event.target as HTMLElement;
    if (target.closest('.menu-panel') || target.closest('.menu-group')) {
      return;
    }
    closeMenus();
  });

  loadImages()
    .then((images) => {
      boardView = new BoardView(canvas, images, (msg) => {
        statusBar.textContent = msg;
      });
    })
    .catch((error) => {
      statusBar.textContent = error instanceof Error ? error.message : 'Failed to load images';
    });
}

function createAboutDialog(): HTMLDivElement {
  const dialog = document.createElement('div');
  dialog.className = 'about-dialog';
  dialog.style.display = 'none';

  const content = document.createElement('div');
  content.className = 'content';
  content.innerHTML = `
    <div><strong>Othello v0.9</strong></div>
    <div style="margin-top: 8px;">A Qt Othello Game.</div>
    <div style="margin-top: 8px;">Ilmari Heikkinen</div>
    <div>Jari Väisänen</div>
  `;

  const button = document.createElement('button');
  button.textContent = 'Close';
  button.addEventListener('click', () => {
    dialog.style.display = 'none';
  });

  content.appendChild(button);
  dialog.appendChild(content);

  return dialog;
}

function showAbout(dialog: HTMLDivElement): void {
  dialog.style.display = 'flex';
}
