import { test, expect } from '@playwright/test';

test('loads and shows a status message', async ({ page }) => {
  await page.goto('/');
  await expect(page.locator('.status-bar')).toContainText('turn');
});

test('can open Game menu and start Two Player Game', async ({ page }) => {
  await page.goto('/');
  await page.getByRole('button', { name: 'Game' }).click();
  await page.getByRole('menuitem', { name: 'Two Player Game', exact: true }).click();
  await expect(page.locator('.status-bar')).toContainText('turn');
});
