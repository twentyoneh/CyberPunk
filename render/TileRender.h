#pragma once
#include "raylib.h"
#include "TileAtlas.h"

// Отрисовка top-down сцены реальными текстурами (вместо цветных заглушек,
// как было в старой изометрической версии). Все функции просто рисуют
// готовый Texture2D в нужном месте экрана — вся сложность (какую текстуру
// выбрать, какой кадр анимации) уже решена в TileAtlas.
namespace TileRender
{
    // topLeft — экранные координаты левого верхнего угла тайла (см. World::GridToScreen).
    void DrawFloor(Vector2 topLeft, const Texture2D& texture);
    void DrawWall(Vector2 topLeft, const Texture2D& texture);

    // feetPos — точка на экране, где находятся "ноги" персонажа (центр нижнего
    // края тайла, на котором он стоит). Спрайт рисуется так, чтобы ноги
    // совпадали с этой точкой, а тело уходило вверх.
    //
    // tint — используется одна и та же текстура и для игрока, и для NPC
    // (в resources пока только один спрайт персонажа), поэтому tint позволяет
    // визуально отличить их друг от друга без второго ассета (например, NPC
    // рисуется с розоватым оттенком).
    void DrawPlayer(Vector2 feetPos, const Texture2D& sheet, Rectangle frameRect, Color tint = WHITE);

    // Иконка "можно поговорить" над головой NPC. abovePos — точка чуть выше
    // головы персонажа, откуда рисуется иконка (центрируется по X от неё).
    void DrawInteractHint(Vector2 abovePos, const Texture2D& icon);
}
