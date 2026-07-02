#pragma once
#include "../game/Enemy.h"
#include "../character/RoleStats.h"
#include <vector>
#include <cstdlib>

// Готовые наборы врагов для сцен — тот же приём, что и NpcRoster.h/TestMaps.h:
// добавить врага значит дописать запись в список ниже, а не трогать main.cpp.
// Каждый враг строится тем же конструктором Character(имя, роль, d10), что и
// игрок в main.cpp, — бросок d10 здесь и даёт разброс статов между врагами
// одной роли (см. RoleStats::roll).
namespace EnemyRoster
{
    inline std::vector<Enemy> NeonAlleyEnemies()
    {
        return {
            {
                14, 6,
                Character("Уличный отморозок", RoleStats::Role::Nomad, rand() % 10 + 1)
            },
            // Ещё один враг — просто ещё одна запись в этом списке, например:
            // {
            //     16, 6,
            //     Character("Корпо-охранник", RoleStats::Role::Lawman, rand() % 10 + 1)
            // },
        };
    }
}
