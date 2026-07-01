#pragma once
#include "AbilityScores.h"
#include "RoleStats.h"
#include "Skill.h"
#include <string>
#include <cmath>
#include <vector>

class Character {
public:
    // Производные характеристики
    int getMaxHP() const {
        return 10 + 5 * (int)std::ceil((stats.BODY + stats.WILL) / 2.0);
    }

    int getHumanityMax() const {
        return stats.EMP * 10;
    }

    void print() const {
        std::cout << "Character: " << name << std::endl;
        std::cout << "Role: " << RoleStats::roleName(role) << std::endl;
        stats.print();
        std::cout << "Max HP: " << getMaxHP() << std::endl;
        std::cout << "Max Humanity: " << getHumanityMax() << std::endl;
    }

    // Конструктор: имя + роль + бросок
    Character(const std::string& name, RoleStats::Role role, int d10)
        : name(name), role(role), stats(RoleStats::roll(role, d10))
    {}
private:
    std::string name;
    RoleStats::Role role;
    AbilityScores stats;
    int currentHP = getMaxHP();
    // std::vector<Skill> skills; // Надо подумать над навыками

};
