#pragma once
#include "Inventory.h"
#include "RoleStats.h"
#include <vector>

class RoleEquipment {
public:
    using Role = RoleStats::Role;

    // choices[i] — индекс выбора для i-го пункта "или" в снаряжении роли
    // Если вектор короче числа выборов — по умолчанию берётся вариант 0
    static Inventory getInventory(Role role, const std::vector<int>& choices = {});

private:
    static int getChoice(const std::vector<int>& choices, int index, int defaultVal = 0);
};
