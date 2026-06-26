#pragma once
#include <string>

enum class GrenadeType {
    Flashbang,   // светошумовая
    Smoke,       // дымовая
    Teargas,     // слезоточивый газ
    Incendiary,  // зажигательная
    Fragmentation,
};

struct Grenade {
    std::string  name;
    GrenadeType  type;
    int          quantity = 1;
};
