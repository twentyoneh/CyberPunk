#include "TileAtlas.h"
#include <cstddef>

namespace
{
    // Размер одного кадра персонажа внутри исходного спрайт-листа
    // (см. resources/Metadata/METADATA.txt: 32x48 на кадр, 3 колонки x 4 строки).
    constexpr int PLAYER_FRAME_WIDTH  = 32;
    constexpr int PLAYER_FRAME_HEIGHT = 48;
}

TileAtlas::TileAtlas()
{
    // Несколько вариантов пола для визуального разнообразия — без этого
    // сплошной ряд одинаковых тайлов выглядит очень "искусственно".
    floorTextures.push_back(LoadTexture("Tiles/floor_concrete_clean_base.png"));
    floorTextures.push_back(LoadTexture("Tiles/floor_concrete_clean_damaged.png"));
    floorTextures.push_back(LoadTexture("Tiles/floor_concrete_clean_steel_blue.png"));

    wallTexture         = LoadTexture("Tiles/wall_interior_base.png");
    playerTexture       = LoadTexture("Characters/npc_01_street_punk.png");
    speechBubbleTexture = LoadTexture("UI/dlg_speech_bubble.png");

    // POINT = без сглаживания при масштабировании, иначе честный пиксель-арт
    // превратится в мыльную кашу при увеличении под низкое разрешение сцены.
    for (Texture2D& t : floorTextures) SetTextureFilter(t, TEXTURE_FILTER_POINT);
    SetTextureFilter(wallTexture, TEXTURE_FILTER_POINT);
    SetTextureFilter(playerTexture, TEXTURE_FILTER_POINT);
    SetTextureFilter(speechBubbleTexture, TEXTURE_FILTER_POINT);
}

TileAtlas::~TileAtlas()
{
    for (Texture2D& t : floorTextures) UnloadTexture(t);
    UnloadTexture(wallTexture);
    UnloadTexture(playerTexture);
    UnloadTexture(speechBubbleTexture);
}

const Texture2D& TileAtlas::GetFloorTexture(int gx, int gy) const
{
    // Простое детерминированное псевдослучайное число из координат клетки:
    // умножаем на разные "простые" множители и суммируем, чтобы соседние
    // клетки обычно получали разные варианты пола.
    int hash = gx * 928371 + gy * 123457;
    size_t index = static_cast<size_t>(hash) % floorTextures.size();
    return floorTextures[index];
}

Rectangle TileAtlas::GetPlayerFrameRect(int direction, int frame) const
{
    return Rectangle{
        (float)(frame * PLAYER_FRAME_WIDTH),
        (float)(direction * PLAYER_FRAME_HEIGHT),
        (float)PLAYER_FRAME_WIDTH,
        (float)PLAYER_FRAME_HEIGHT
    };
}
