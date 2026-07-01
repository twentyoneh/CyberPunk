#pragma once
#include "Npc.h"
#include <vector>

// Владеет всеми NPC текущей сцены и отвечает на вопросы о них — main.cpp
// больше не обращается к конкретным NPC по имени переменной (streetPunk и
// т.п.), а спрашивает у NpcManager: "занята ли эта клетка", "есть ли рядом
// с игроком кто-то, с кем можно заговорить". Добавление нового NPC — это
// правка данных в NpcRoster.h (см. src/world/NpcRoster.h), а не правка кода
// в main.cpp: ни цикл движения, ни отрисовка не знают, сколько там NPC.
//
// ВАЖНО: NpcManager не поддерживает добавление/удаление NPC после создания —
// вектор npcs не меняет размер, поэтому указатели на его элементы (их
// возвращает FindInteractable, их хранит Dialogue, пока идёт разговор)
// остаются действительными весь срок жизни менеджера.
class NpcManager
{
public:
    explicit NpcManager(std::vector<Npc> npcs);

    // true, если клетка (x, y) занята каким-нибудь NPC — используется в
    // main.cpp наравне с Grid::IsWalkable при проверке, можно ли туда шагнуть.
    bool IsOccupied(int x, int y) const;

    // NPC, к которому игрок стоит вплотную (ровно 1 по Манхэттену: по одной
    // из 4 сторон, не по диагонали), или nullptr, если рядом никого нет.
    const Npc* FindInteractable(int playerGX, int playerGY) const;

    // Для отрисовки — main.cpp просто проходит по всем NPC одним циклом,
    // не заботясь о том, сколько их и какие они (см. main.cpp).
    const std::vector<Npc>& All() const { return npcs; }

private:
    std::vector<Npc> npcs;
};
