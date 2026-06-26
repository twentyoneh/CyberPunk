#pragma once
#include "AbilityScores.h"
#include "RoleStats.h"
#include "RoleSkills.h"
#include "RoleEquipment.h"
#include <string>
#include <vector>
#include <cmath>
#include <iostream>

class Character {
public:
    std::string      name;
    RoleStats::Role  role;
    AbilityScores    stats;
    std::vector<Skill> skills;
    Inventory        inventory;

    // choices — индексы выборов "или" для снаряжения (см. RoleEquipment)
    Character(const std::string& name, RoleStats::Role role, int d10,
              const std::vector<int>& choices = {})
        : name(name)
        , role(role)
        , stats(RoleStats::roll(role, d10))
        , skills(RoleSkills::getSkills(role))
        , inventory(RoleEquipment::getInventory(role, choices))
    {}

    int getMaxHP() const {
        return 10 + 5 * (int)std::ceil((stats.BODY + stats.WILL) / 2.0);
    }

    int getHumanityMax() const {
        return stats.EMP * 10;
    }

    void print() const {
        std::cout << "=== " << name
                  << " [" << RoleStats::roleName(role) << "] ===\n";
        stats.print();
        std::cout << "HP макс: "           << getMaxHP()       << "\n";
        std::cout << "Человечность макс: " << getHumanityMax() << "\n";
        std::cout << "\n--- Навыки ---\n";
        for (const auto& s : skills)
            std::cout << s.name << ": " << s.level << "\n";
        std::cout << "\n";
        inventory.print();
    }
};
