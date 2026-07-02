#include "BattleRender.h"
#include <string>

namespace
{
    // Клетка арены крупнее клетки карты исследования (World::TILE_SIZE == 16) —
    // арена маленькая (по умолчанию 10x6, см. Battle.h), поэтому можно себе
    // позволить укрупнить клетку, чтобы юниты и полоски HP читались.
    constexpr int CELL_SIZE = 24;

    Vector2 ArenaOrigin(const Battle& battle, int canvasWidth, int canvasHeight)
    {
        float arenaPixelW = battle.GetArenaWidth()  * (float)CELL_SIZE;
        float arenaPixelH = battle.GetArenaHeight() * (float)CELL_SIZE;
        return Vector2{
            (canvasWidth  - arenaPixelW) / 2.0f,
            (canvasHeight - arenaPixelH) / 2.0f - 12.0f // сдвиг вверх — снизу место под лог боя
        };
    }

    void DrawArenaGrid(const Battle& battle, Vector2 origin)
    {
        for (int y = 0; y < battle.GetArenaHeight(); y++)
        {
            for (int x = 0; x < battle.GetArenaWidth(); x++)
            {
                Rectangle cell = {
                    origin.x + x * CELL_SIZE,
                    origin.y + y * CELL_SIZE,
                    (float)CELL_SIZE - 1,
                    (float)CELL_SIZE - 1
                };
                DrawRectangleRec(cell, Color{ 30, 30, 45, 255 });
                DrawRectangleLinesEx(cell, 1, Color{ 60, 60, 80, 255 });
            }
        }
    }

    void DrawUnits(const Battle& battle, Vector2 origin)
    {
        const std::vector<CombatUnit>& units = battle.GetUnits();
        int currentIndex = battle.GetCurrentUnitIndex();

        for (int i = 0; i < (int)units.size(); i++)
        {
            const CombatUnit& u = units[i];
            bool alive = u.character->isAlive();

            Vector2 cellTopLeft = { origin.x + u.ax * CELL_SIZE, origin.y + u.ay * CELL_SIZE };
            Vector2 center = { cellTopLeft.x + CELL_SIZE / 2.0f, cellTopLeft.y + CELL_SIZE / 2.0f };

            // Подсветка клетки того юнита, чей сейчас ход — золотая рамка,
            // чтобы было видно, кто ходит, даже без отдельного текста.
            if (i == currentIndex && alive)
            {
                Rectangle highlight = { cellTopLeft.x, cellTopLeft.y, (float)CELL_SIZE - 1, (float)CELL_SIZE - 1 };
                DrawRectangleLinesEx(highlight, 2, GOLD);
            }

            Color unitColor;
            if (!alive)                unitColor = Color{ 90, 90, 90, 255 };   // побеждён — серый
            else if (u.isPlayer)       unitColor = Color{ 100, 200, 255, 255 }; // игрок — голубой
            else                       unitColor = Color{ 233, 69, 96, 255 };   // враг — Neon Coral

            DrawCircleV(center, CELL_SIZE / 2.0f - 4, unitColor);

            if (!alive) continue; // у побеждённого юнита полоску HP не рисуем

            // Полоска HP над юнитом.
            float hpRatio = (float)u.character->getCurrentHP() / (float)u.character->getMaxHP();
            if (hpRatio < 0.0f) hpRatio = 0.0f;

            Rectangle hpBack = { cellTopLeft.x + 2, cellTopLeft.y - 5, (float)CELL_SIZE - 5, 3 };
            DrawRectangleRec(hpBack, Fade(BLACK, 0.6f));

            Rectangle hpFront = hpBack;
            hpFront.width *= hpRatio;
            Color hpColor = (hpRatio > 0.5f) ? GREEN : (hpRatio > 0.25f ? YELLOW : RED);
            DrawRectangleRec(hpFront, hpColor);
        }
    }

    void DrawLog(const Battle& battle, const Font& font, int canvasWidth, int canvasHeight)
    {
        constexpr int LOG_HEIGHT = 40;
        constexpr int MAX_VISIBLE_LINES = 3;
        constexpr int FONT_SIZE = 9;

        Rectangle box = {
            6.0f,
            (float)(canvasHeight - LOG_HEIGHT - 6),
            (float)(canvasWidth - 12),
            (float)LOG_HEIGHT
        };

        DrawRectangleRec(box, Fade(BLACK, 0.8f));
        DrawRectangleLinesEx(box, 1, Color{ 0, 255, 245, 255 }); // Arc Cyan — тот же акцент, что и DialogueBox

        const std::vector<std::string>& log = battle.GetLog();
        int start = (int)log.size() > MAX_VISIBLE_LINES ? (int)log.size() - MAX_VISIBLE_LINES : 0;

        float lineY = box.y + 3;
        for (int i = start; i < (int)log.size(); i++)
        {
            DrawTextEx(font, log[i].c_str(), Vector2{ box.x + 4, lineY }, FONT_SIZE, 1, Fade(RAYWHITE, 0.9f));
            lineY += FONT_SIZE + 2;
        }
    }
}

namespace BattleRender
{
    void Draw(const Battle& battle, const Font& font, int canvasWidth, int canvasHeight)
    {
        Vector2 origin = ArenaOrigin(battle, canvasWidth, canvasHeight);

        DrawArenaGrid(battle, origin);
        DrawUnits(battle, origin);

        // --- Индикатор хода и подсказка по управлению --------------------------
        const char* turnLabel = battle.IsPlayerTurn() ? "Ваш ход" : "Ход противника";
        DrawTextEx(font, turnLabel, Vector2{ 6, 6 }, 10, 1, RAYWHITE);

        if (battle.IsPlayerTurn())
        {
            const char* hint = "Стрелки - движение, Space - атака";
            Vector2 hintSize = MeasureTextEx(font, hint, 9, 1);
            DrawTextEx(font, hint, Vector2{ canvasWidth - hintSize.x - 6, 6 }, 9, 1, Fade(RAYWHITE, 0.7f));
        }

        DrawLog(battle, font, canvasWidth, canvasHeight);
    }
}
