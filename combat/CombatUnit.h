#pragma once
#include "../character/Character.h"

// Один участник боя на арене (см. Battle.h). Не владеет Character — хранит
// указатель на игрока (живёт в main.cpp) либо на Character внутри Enemy
// (живёт в EnemyManager, см. enemyIndex), поэтому урон, нанесённый во время
// боя, сохраняется и после его окончания без какого-либо копирования назад.
struct CombatUnit
{
    int ax; // позиция на боевой арене (не путать с gx/gy на карте исследования)
    int ay;

    Character* character = nullptr;

    bool isPlayer = false;

    // Индекс в EnemyManager, если это враг (см. Battle::MarkDefeatedEnemies).
    // -1 для игрока.
    int enemyIndex = -1;

    // d10 + REF, бросается один раз при старте боя — определяет порядок ходов
    // (см. Battle::BuildInitiativeOrder).
    int initiative = 0;
};
