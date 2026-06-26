#pragma once
#include "Ammo.h"
#include <string>

struct Weapon {
    std::string name;
    AmmoType    ammoType    = AmmoType::None;  // None = холодное оружие
    int         damage      = 0;  // кости урона (напр. 3 = 3d6)
    int         magazine    = 0;  // ёмкость магазина
    int         currentAmmo = 0;  // текущий заряд
};
