#pragma once
#include "raylib.h"
#include "../combat/Battle.h"

// Отрисовка боевой сцены: своя маленькая арена (см. Battle.h) поверх той же
// низкоразрешённой текстуры, что и сцена исследования — просто вместо
// тайлов карты и NPC рисуется сетка арены, юниты (кружки с полоской HP) и
// лог боя. Ничего не знает о правилах боя — только читает состояние Battle.
namespace BattleRender
{
    // canvasWidth/canvasHeight — размер низкоразрешённого холста (в main.cpp
    // это RENDER_WIDTH/RENDER_HEIGHT).
    void Draw(const Battle& battle, const Font& font, int canvasWidth, int canvasHeight);
}
