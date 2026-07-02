#pragma once
#include "Weapon.h"
#include "Armor.h"
#include "Grenade.h"
#include "GearItem.h"
#include "Clothing.h"
#include <vector>
#include <map>
#include <iostream>
#include <algorithm> // std::min (используется в reload())

struct Inventory {
    std::vector<Weapon>         weapons;
    std::vector<Armor>          armor;
    std::map<AmmoType, int>     ammo;
    std::vector<Grenade>        grenades;
    std::vector<GearItem>       gear;
    std::vector<ClothingSet>    clothing;

    // Добавить патроны в общий пул
    void addAmmo(AmmoType type, int count) {
        ammo[type] += count;
    }

    // Выстрел: списать патрон из оружия и пула
    bool shoot(Weapon& weapon) {
        if (weapon.currentAmmo <= 0) return false;
        weapon.currentAmmo--;
        return true;
    }

    // Перезарядка: добавить патроны из пула в оружие
    bool reload(Weapon& weapon) {
        if (weapon.ammoType == AmmoType::None) return false;
        int needed = weapon.magazine - weapon.currentAmmo;
        int available = ammo[weapon.ammoType];
        int take = std::min(needed, available);
        weapon.currentAmmo += take;
        ammo[weapon.ammoType] -= take;
        return take > 0;
    }

    void print() const {
        std::cout << "--- Оружие ---\n";
        for (const auto& w : weapons)
            std::cout << w.name << "  [" << w.currentAmmo << "/" << w.magazine << "]\n";

        std::cout << "--- Броня ---\n";
        for (const auto& a : armor)
            std::cout << a.name << "  СП:" << a.stoppingPower << "\n";

        std::cout << "--- Патроны ---\n";
        for (const auto& [type, count] : ammo)
            if (count > 0)
                std::cout << ammoTypeName(type) << ": " << count << "\n";

        std::cout << "--- Гранаты ---\n";
        for (const auto& g : grenades)
            std::cout << g.name << " x" << g.quantity << "\n";

        std::cout << "--- Снаряжение ---\n";
        for (const auto& item : gear)
            std::cout << item.name << (item.quantity > 1 ? " x" + std::to_string(item.quantity) : "") << "\n";

        std::cout << "--- Одежда ---\n";
        for (const auto& set : clothing) {
            std::cout << clothingStyleName(set.style) << ": ";
            for (size_t i = 0; i < set.pieces.size(); i++) {
                const auto& p = set.pieces[i];
                std::cout << p.name << (p.quantity > 1 ? " x" + std::to_string(p.quantity) : "");
                if (i + 1 < set.pieces.size()) std::cout << ", ";
            }
            std::cout << "\n";
        }
    }
};
