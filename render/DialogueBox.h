#pragma once
#include "raylib.h"
#include "../game/Dialogue.h"

// Отрисовка диалогового окна поверх сцены. Ничего не знает про то, как
// устроен сам диалог (Dialogue) — только читает его текущее состояние и рисует.
namespace DialogueBox
{
    // canvasWidth/canvasHeight — размер низкоразрешённого холста сцены
    // (в main.cpp это RENDER_WIDTH/RENDER_HEIGHT). Вызывать только когда
    // dialogue.IsOpen() == true.
    void Draw(const Dialogue& dialogue, const Font& font, int canvasWidth, int canvasHeight);
}
