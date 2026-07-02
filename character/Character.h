#pragma once
#include "AbilityScores.h"
#include "RoleStats.h"
#include "RoleSkills.h"
#include "RoleEquipment.h"
#include "Skill.h"
#include "Inventory.h"
#include <string>
#include <cmath>
#include <vector>
#include <iostream>

// Персонаж: статы + роль + навыки + инвентарь в одном месте. Раньше навыки и
// инвентарь существовали как отдельные системы (RoleSkills/RoleEquipment), но
// никак не были связаны с Character — конструктор ниже связывает всё разом,
// поэтому и игрок, и враги (см. src/combat/EnemyRoster.h) собираются одним и
// тем же способом и одинаково пригодны для боевой системы.
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

    // --- Доступ для боевой системы (см. src/combat/CombatMath.h) -----------
    const std::string& getName() const { return name; }
    RoleStats::Role getRole() const { return role; }
    const AbilityScores& getStats() const { return stats; }
    const std::vector<Skill>& getSkills() const { return skills; }

    Inventory& getInventory() { return inventory; }
    const Inventory& getInventory() const { return inventory; }

    int getCurrentHP() const { return currentHP; }
    bool isAlive() const { return currentHP > 0; }

    // Наносит урон, не пуская HP в минус (удобно для полосок здоровья и т.п.).
    void takeDamage(int amount) {
        if (amount <= 0) return;
        currentHP -= amount;
        if (currentHP < 0) currentHP = 0;
    }

    // Восстанавливает HP, не превышая максимум. Пока используется как грубая
    // заглушка "лечения" после поражения в бою (см. main.cpp) — отдельной
    // сцены game over/лечения ещё нет.
    void heal(int amount) {
        if (amount <= 0) return;
        currentHP += amount;
        if (currentHP > getMaxHP()) currentHP = getMaxHP();
    }

    // Уровень навыка по имени, 0 — если персонаж им не владеет (не ошибка,
    // а честный результат: например, у Нетраннера нет "Оружие ближнего боя").
    int getSkillLevel(const std::string& skillName) const {
        for (const Skill& s : skills)
            if (s.name == skillName) return s.level;
        return 0;
    }

    // Конструктор: имя + роль + бросок 1d10. Сразу же генерирует навыки
    // (RoleSkills) и стартовое снаряжение (RoleEquipment) для этой роли —
    // отдельно вызывать их самому не нужно.
    Character(const std::string& name, RoleStats::Role role, int d10)
        : name(name)
        , role(role)
        , stats(RoleStats::roll(role, d10))
        , skills(RoleSkills::getSkills(role))
        , inventory(RoleEquipment::getInventory(role))
    {
        currentHP = getMaxHP();
    }

private:
    std::string name;
    RoleStats::Role role;
    AbilityScores stats;
    int currentHP;
    std::vector<Skill> skills;
    Inventory inventory;
};
