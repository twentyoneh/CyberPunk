#pragma once
#include "AbilityScores.h"
#include <string>

class RoleStats {
public:
    enum class Role {
        Rockerboy = 0,
        Solo,
        Netrunner,
        Tech,
        Medtech,
        Media,
        Lawman,
        Corp,
        Fixer,
        Nomad,
        COUNT
    };

    // Возвращает блок статов для роли по броску 1d10
    static AbilityScores roll(Role role, int d10);

    static std::string roleName(Role role);

private:
    // TABLE[role][d10-1] -> AbilityScores
    // Порядок полей: INT_, REF, DEX, TECH, COOL, WILL, LUCK, MOVE, BODY, EMP
    static const AbilityScores TABLE[static_cast<int>(Role::COUNT)][10];
};
