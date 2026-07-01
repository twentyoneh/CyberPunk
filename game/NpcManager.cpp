#include "NpcManager.h"
#include <cstdlib>

NpcManager::NpcManager(std::vector<Npc> npcs)
    : npcs(std::move(npcs))
{
}

bool NpcManager::IsOccupied(int x, int y) const
{
    for (const Npc& npc : npcs)
    {
        if (npc.gx == x && npc.gy == y)
            return true;
    }
    return false;
}

const Npc* NpcManager::FindInteractable(int playerGX, int playerGY) const
{
    for (const Npc& npc : npcs)
    {
        int distX = std::abs(playerGX - npc.gx);
        int distY = std::abs(playerGY - npc.gy);

        if (distX + distY == 1)
            return &npc;
    }
    return nullptr;
}
