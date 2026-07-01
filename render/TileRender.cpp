#include "TileRender.h"

namespace TileRender
{
    void DrawFloor(Vector2 topLeft, const Texture2D& texture)
    {
        // Тайлы ассетов ровно совпадают по размеру с World::TILE_SIZE,
        // поэтому текстуру можно рисовать "один в один", без масштабирования.
        DrawTextureV(texture, topLeft, WHITE);
    }

    void DrawWall(Vector2 topLeft, const Texture2D& texture)
    {
        DrawTextureV(texture, topLeft, WHITE);
    }

    void DrawPlayer(Vector2 feetPos, const Texture2D& sheet, Rectangle frameRect, Color tint)
    {
        // Спрайт персонажа шире и выше одного тайла (32x48 против 16x16),
        // поэтому его нужно не просто "положить" в клетку, а выровнять по
        // центру и по низу: X — по центру тайла, Y — так, чтобы низ спрайта
        // (ноги) совпадал с feetPos, а тело уходило вверх, "стоя" на тайле.
        Rectangle dest = {
            feetPos.x - frameRect.width  / 2.0f,
            feetPos.y - frameRect.height,
            frameRect.width,
            frameRect.height
        };

        DrawTexturePro(sheet, frameRect, dest, Vector2{ 0, 0 }, 0.0f, tint);
    }

    void DrawInteractHint(Vector2 abovePos, const Texture2D& icon)
    {
        // Иконка рисуется чуть уменьшенной (исходная 32x32 — многовато для
        // холста 480x270) и центрированной по X относительно abovePos.
        float scale = 0.5f;
        Rectangle src  = { 0, 0, (float)icon.width, (float)icon.height };
        Rectangle dest = {
            abovePos.x - (icon.width * scale) / 2.0f,
            abovePos.y - icon.height * scale,
            icon.width * scale,
            icon.height * scale
        };
        DrawTexturePro(icon, src, dest, Vector2{ 0, 0 }, 0.0f, WHITE);
    }
}
