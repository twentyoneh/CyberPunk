#pragma once
#include "raylib.h"
#include <vector>

// Загружает и хранит все текстуры, нужные для top-down сцены (тайлы + спрайт
// персонажа из resources/). Всё грузится один раз при старте и выгружается
// в деструкторе (RAII) — благодаря этому в main.cpp не нужно помнить самому
// вызывать UnloadTexture на каждую картинку.
//
// ВАЖНО: конструктор вызывает LoadTexture, а это требует, чтобы окно/OpenGL
// контекст уже были созданы (InitWindow). Поэтому TileAtlas создаётся в main()
// уже ПОСЛЕ InitWindow, не раньше.
class TileAtlas
{
public:
    TileAtlas();
    ~TileAtlas();

    // Текстуры — это "живые" хендлы ресурсов на GPU, копировать такой объект
    // нельзя (иначе при двойном UnloadTexture будет двойное освобождение).
    TileAtlas(const TileAtlas&) = delete;
    TileAtlas& operator=(const TileAtlas&) = delete;

    // Текстура пола для конкретной клетки. Вариант выбирается детерминированно
    // по координатам клетки (см. .cpp) — одна и та же клетка всегда получает
    // один и тот же вариант, пол не "мерцает" на каждом кадре.
    const Texture2D& GetFloorTexture(int gx, int gy) const;

    const Texture2D& GetWallTexture() const { return wallTexture; }

    const Texture2D& GetPlayerTexture() const { return playerTexture; }

    // Прямоугольник одного кадра персонажа внутри спрайт-листа npc_01_street_punk.png.
    // direction: 0 = юг (вниз), 1 = запад (влево), 2 = восток (вправо), 3 = север (вверх)
    //            — именно в таком порядке лежат строки в файле (см. resources/Metadata/METADATA.txt).
    // frame:     0 = стойка (idle), 1 = шаг левой, 2 = шаг правой.
    Rectangle GetPlayerFrameRect(int direction, int frame) const;

    // Иконка "можно поговорить" (пузырь речи) — рисуется над головой NPC,
    // когда игрок стоит рядом и ещё не начал диалог.
    const Texture2D& GetSpeechBubbleTexture() const { return speechBubbleTexture; }

private:
    std::vector<Texture2D> floorTextures;
    Texture2D wallTexture{};
    Texture2D playerTexture{};
    Texture2D speechBubbleTexture{};
};
