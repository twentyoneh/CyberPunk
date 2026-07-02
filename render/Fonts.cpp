#include "Fonts.h"
#include <vector>

namespace Fonts
{
    Font LoadCyrillicFont(const char* path, int fontSize)
    {
        // Явно перечисляем нужные диапазоны юникода. Если не передать список
        // самому, LoadFontEx возьмёт только первые ~95 кодов (обычная латиница) —
        // этого недостаточно для русского текста.
        std::vector<int> codepoints;

        for (int c = 0x0020; c <= 0x007E; c++) codepoints.push_back(c); // базовая латиница, цифры, пунктуация

        // Кириллица: берём только реально используемый диапазон (современный
        // русский А-я/Ё-ё + украинские/белорусские варианты) — до 0x045F, а не
        // весь блок до 0x04FF. Буквы после 0x0460 — устаревшие/старославянские,
        // которых у большинства шрифтов (в т.ч. у DejaVu Sans Mono) просто нет:
        // они превращаются в "квадратик" (глиф-заглушку), который зря занимает
        // место в атласе и не нужен для русского текста.
        for (int c = 0x0400; c <= 0x045F; c++) codepoints.push_back(c);

        // Немного типографской пунктуации, которая часто встречается в русском
        // тексте, но не входит в диапазоны выше (обычное тире "-" уже есть в ASCII).
        codepoints.push_back(0x2014); // — длинное тире
        codepoints.push_back(0x2013); // – короткое тире
        codepoints.push_back(0x00AB); // «
        codepoints.push_back(0x00BB); // »

        // Раньше здесь был просто LoadFontEx(path, fontSize, codepoints.data(), count) —
        // но у LoadFontEx внутри жёстко зашит packMethod=0 при сборке атласа
        // (см. GenImageFontAtlas в rtext.c raylib): это НАИВНАЯ построчная
        // раскладка глифов, которая резервирует под каждую строку атласа
        // фиксированную высоту ровно fontSize+2*padding — как будто ВСЕ глифы
        // одной высоты. На практике это не так: у любого шрифта попадаются
        // глифы (акценты, некоторые кириллические буквы) реальной высотой
        // БОЛЬШЕ fontSize, и их нижние пиксели просто съезжают в область
        // следующей строки атласа — на экране это выглядит как горизонтальные
        // полосы/обрывки чужих букв прямо поперёк текста (именно это и было).
        //
        // Чтобы обойти баг, собираем Font вручную теми же шагами, что и
        // LoadFontEx/LoadFontFromMemory внутри raylib, но с packMethod=1 —
        // это честная Skyline-раскладка (stb_rect_pack), которая учитывает
        // РЕАЛЬНУЮ ширину И высоту каждого глифа и никогда не даёт им
        // перекрываться.
        int fileSize = 0;
        unsigned char* fileData = LoadFileData(path, &fileSize);

        Font font = { 0 };
        font.baseSize = fontSize;
        font.glyphPadding = 4; // то же значение, что FONT_TTF_DEFAULT_CHARS_PADDING в raylib

        font.glyphs = LoadFontData(fileData, fileSize, font.baseSize,
                                    codepoints.data(), (int)codepoints.size(),
                                    FONT_DEFAULT, &font.glyphCount);

        Image atlas = GenImageFontAtlas(font.glyphs, &font.recs, font.glyphCount,
                                         font.baseSize, font.glyphPadding, 1); // 1 = Skyline-раскладка
        font.texture = LoadTextureFromImage(atlas);

        for (int i = 0; i < font.glyphCount; i++)
        {
            UnloadImage(font.glyphs[i].image);
            font.glyphs[i].image = ImageFromImage(atlas, font.recs[i]);
        }

        UnloadImage(atlas);
        UnloadFileData(fileData);

        // POINT — без сглаживания, чтобы текст не выбивался по стилю из остального
        // пиксель-арта (та же логика, что и для тайлов/спрайта персонажа).
        SetTextureFilter(font.texture, TEXTURE_FILTER_POINT);

        return font;
    }
}
