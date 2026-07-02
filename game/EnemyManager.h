#pragma once
#include "Enemy.h"
#include <vector>

// Владеет всеми врагами текущей сцены — тот же приём, что и NpcManager для
// NPC (см. NpcManager.h). Разница в одном: враги могут быть побеждены, поэтому
// вместо возврата указателя/ссылки на найденного врага (как FindInteractable
// у NPC) здесь возвращается ИНДЕКС (см. FindEngageableIndex) — после победы
// в бою main.cpp помечает врага побеждённым по этому индексу через
// MarkDefeated, а не хранит указатель, который пришлось бы инвалидировать
// вручную.
//
// ВАЖНО: как и у NpcManager, вектор enemies не меняет размер после создания
// (побеждённые враги не удаляются, а помечаются флагом Enemy::defeated) —
// поэтому индексы остаются стабильными весь срок жизни менеджера.
class EnemyManager
{
public:
    explicit EnemyManager(std::vector<Enemy> enemies);

    // true, если клетка (x, y) занята каким-нибудь ЖИВЫМ врагом — побеждённые
    // враги клетку больше не блокируют.
    bool IsOccupied(int x, int y) const;

    // Индекс живого врага ровно в клетке (x, y), или -1. В отличие от
    // FindEngageableIndex (расстояние == 1, "рядом"), здесь расстояние == 0
    // ("прямо на этой клетке") — используется в main.cpp, чтобы понять, что
    // игрок толкнулся именно во врага, а не просто идёт мимо, и запустить Battle.
    int FindIndexAt(int x, int y) const;

    // Индекс живого врага, к которому игрок стоит вплотную (Манхэттен == 1),
    // или -1, если рядом никого нет. Используется и для подсказки "завязать
    // бой", и для запуска самого Battle (см. main.cpp).
    int FindEngageableIndex(int playerGX, int playerGY) const;

    Enemy& Get(int index) { return enemies[index]; }
    const Enemy& Get(int index) const { return enemies[index]; }

    void MarkDefeated(int index) { enemies[index].defeated = true; }

    // Для отрисовки — тот же цикл-по-всем, что и у NpcManager::All().
    const std::vector<Enemy>& All() const { return enemies; }

private:
    std::vector<Enemy> enemies;
};
