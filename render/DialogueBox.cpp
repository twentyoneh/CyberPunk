#include "DialogueBox.h"

namespace DialogueBox
{
    void Draw(const Dialogue& dialogue, const Font& font, int canvasWidth, int canvasHeight)
    {
        constexpr int MARGIN     = 6;
        constexpr int BOX_HEIGHT = 44;
        constexpr int FONT_SIZE  = 10;

        Rectangle box = {
            (float)MARGIN,
            (float)(canvasHeight - BOX_HEIGHT - MARGIN),
            (float)(canvasWidth - MARGIN * 2),
            (float)BOX_HEIGHT
        };

        // Тёмная полупрозрачная подложка + неоновая рамка — чтобы окно читалось
        // поверх сцены, но не выглядело чужеродным на фоне остального арта.
        DrawRectangleRec(box, Fade(BLACK, 0.8f));
        DrawRectangleLinesEx(box, 1, Color{ 0, 255, 245, 255 }); // Arc Cyan из палитры resources/Metadata

        Vector2 namePos = { box.x + 8, box.y + 4 };
        Vector2 linePos = { box.x + 8, box.y + 18 };

        DrawTextEx(font, dialogue.SpeakerName().c_str(), namePos, FONT_SIZE, 1, Color{ 233, 69, 96, 255 }); // Neon Coral
        DrawTextEx(font, dialogue.CurrentLine().c_str(), linePos, FONT_SIZE, 1, RAYWHITE);

        // Подсказка в правом нижнем углу окна — какая клавиша продолжает диалог.
        const char* hint = dialogue.IsLastLine() ? "Enter - закрыть" : "Enter - дальше";
        Vector2 hintSize = MeasureTextEx(font, hint, FONT_SIZE, 1);
        Vector2 hintPos = { box.x + box.width - hintSize.x - 8, box.y + box.height - hintSize.y - 4 };
        DrawTextEx(font, hint, hintPos, FONT_SIZE, 1, Fade(RAYWHITE, 0.7f));
    }
}
