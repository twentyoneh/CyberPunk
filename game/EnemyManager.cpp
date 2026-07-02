#include "EnemyManager.h"
#include <cstdlib>

EnemyManager::EnemyManager(std::vector<Enemy> enemies)
    : enemies(std::move(enemies))
{
}

bool EnemyManager::IsOccupied(int x, int y) const
{
    return FindIndexAt(x, y) != -1;
}

int EnemyManager::FindIndexAt(int x, int y) const
{
    for (int i = 0; i < (int)enemies.size(); i++)
    {
        if (enemies[i].defeated) continue;
        if (enemies[i].gx == x && enemies[i].gy == y)
            return i;
    }
    return -1;
}

int EnemyManager::FindEngageableIndex(int playerGX, int playerGY) const
{
    for (int i = 0; i < (int)enemies.size(); i++)
    {
        const Enemy& enemy = enemies[i];
        if (enemy.defeated) continue;

        int distX = std::abs(playerGX - enemy.gx);
        int distY = std::abs(playerGY - enemy.gy);

        if (distX + distY == 1)
            return i;
    }
    return -1;
}
