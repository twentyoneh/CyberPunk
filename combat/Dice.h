#pragma once
#include <cstdlib>

// Единая точка для всех бросков кубиков в игре. Раньше rand() вызывался
// напрямую в разных местах (main.cpp, Character) — если позже понадобится,
// например, подсчитывать статистику бросков, вести лог "кубик выпал X",
// или подменить генератор на детерминированный для тестов, менять нужно
// только этот файл, а не искать rand() по всему проекту.
namespace Dice
{
    // Бросок кубика с произвольным числом граней, результат 1..sides.
    inline int Roll(int sides)
    {
        return rand() % sides + 1;
    }

    inline int RollD10() { return Roll(10); }
    inline int RollD6()  { return Roll(6);  }

    // Сумма нескольких кубиков d6 — ровно то, что описывает урон оружия
    // в Weapon.h ("3 = 3d6"). count <= 0 трактуется как отсутствие урона.
    inline int RollNd6(int count)
    {
        int total = 0;
        for (int i = 0; i < count; i++)
            total += RollD6();
        return total;
    }
}
