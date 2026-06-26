#pragma once
#include "Skill.h"
#include "RoleStats.h"
#include <vector>

class RoleSkills {
public:
    using Role = RoleStats::Role;

    static std::vector<Skill> getSkills(Role role);
};
