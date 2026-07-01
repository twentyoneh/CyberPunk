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
        for (int c = 0x0400; c <= 0x04FF; c++) codepoints.push_back(c); // весь блок кириллицы (А-Я, а-я, Ё/ё и т.д.)

        // Немного типографской пунктуации, которая часто встречается в русском
        // тексте, но не входит в диапазоны выше (обычное тире "-" уже есть в ASCII).
        codepoints.push_back(0x2014); // — длинное тире
        codepoints.push_back(0x2013); // – короткое тире
        codepoints.push_back(0x00AB); // «
        codepoints.push_back(0x00BB); // »

        Font font = LoadFontEx(path, fontSize, codepoints.data(), (int)codepoints.size());

        // POINT — без сглаживания, чтобы текст не выбивался по стилю из остального
        // пиксель-арта (та же логика, что и для тайлов/спрайта персонажа).
        SetTextureFilter(font.texture, TEXTURE_FILTER_POINT);

        return font;
    }
}
