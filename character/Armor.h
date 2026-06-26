#pragma once
#include <string>

enum class ArmorLocation { Body, Head };

struct Armor {
    std::string   name;
    ArmorLocation location      = ArmorLocation::Body;
    int           stoppingPower = 0;  // СП — сколько урона поглощает
};
