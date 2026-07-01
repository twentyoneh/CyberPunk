#pragma once
#include <string>
#include <vector>

// Простой NPC на карте: стоит на месте (не двигается, не патрулирует — это
// задел на будущее) и хранит свои реплики. Сама логика "кто с кем говорит"
// живёт в Dialogue.h, а не здесь — Npc это просто данные.
struct Npc
{
    int gx;
    int gy;
    std::string name;
    std::vector<std::string> lines; // реплики по порядку, показываются одна за другой
};
