
#include "raylib.h"
#include "resource_dir.h"
#include "./character/RoleStats.h"
#include "./character/Character.h"
#include "./world/Tile.h"
#include "./world/Grid.h"
#include "./world/GridMath.h"
#include "./world/TestMaps.h"
#include "./world/NpcRoster.h"
#include "./render/TileAtlas.h"
#include "./render/TileRender.h"
#include "./render/Fonts.h"
#include "./render/DialogueBox.h"
#include "./game/TurnManager.h"
#include "./game/Npc.h"
#include "./game/NpcManager.h"
#include "./game/Dialogue.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>

// =====================================================================================
// ОБЩАЯ ЛОГИКА ПРОГРАММЫ
// =====================================================================================
// Прототип боевой/игровой сцены поверх уже готовой модели персонажа (Character.h,
// RoleStats.h и т.д. — они просто используются здесь для примера, на экран пока
// не выводятся).
//
// Камера — вид СВЕРХУ (top-down), а не изометрия: клетки сетки рисуются обычными
// квадратами размера World::TILE_SIZE, без поворота на 45 градусов. Сам бой позже
// будет решаться бросками кубика (пока не реализовано) — сейчас есть только
// пошаговое перемещение по сетке, чтобы отработать сцену/рендер/тайлы.
//
// Схема работы каждого кадра:
//   1) INPUT   — читаем нажатие клавиши движения. Один шаг = один "ход" (turnManager).
//   2) RENDER В ТЕКСТУРУ — сцена рисуется в маленькую RenderTexture2D фиксированного
//               размера, а не прямо в окно, — так примитивы/текстуры остаются честным
//               пиксель-артом при любом размере окна (см. GetScaledDestRect ниже).
//   3) ВЫВОД В ОКНО — картинка растягивается на размер окна точечной фильтрацией
//               (без сглаживания), с чёрными полосами по краям (letterbox), если
//               пропорции окна не совпадают со сценой.
//
// "Камера" реализована просто как сдвиг (origin): каждый кадр пересчитываем его так,
// чтобы клетка игрока всегда была в центре экрана.
// =====================================================================================

// Внутреннее разрешение сцены. Чем меньше число — тем крупнее "пиксели" на экране.
static constexpr int RENDER_WIDTH  = 480;
static constexpr int RENDER_HEIGHT = 270;

// Направления персонажа — совпадают по порядку со строками спрайт-листа
// npc_01_street_punk.png (см. resources/Metadata/METADATA.txt).
static constexpr int FACING_SOUTH = 0;
static constexpr int FACING_WEST  = 1;
static constexpr int FACING_EAST  = 2;
static constexpr int FACING_NORTH = 3;

// Сколько секунд после шага показывать кадр "шага" (frame 1/2), прежде чем
// вернуться к спокойной стойке (frame 0). Чисто визуальная "приправа" —
// без неё персонаж на месте выглядел бы как статичная картинка.
static constexpr double STEP_POSE_DURATION = 0.15;

// Вписывает картинку низкого разрешения в окно с сохранением пропорций (letterbox),
// чтобы при любом размере/соотношении сторон окна пиксели оставались квадратными.
static Rectangle GetScaledDestRect(int screenWidth, int screenHeight)
{
    float scale = std::min((float)screenWidth / RENDER_WIDTH, (float)screenHeight / RENDER_HEIGHT);
    if (scale < 1.0f) scale = 1.0f; // не даём картинке становиться меньше нативного размера

    float destWidth  = RENDER_WIDTH * scale;
    float destHeight = RENDER_HEIGHT * scale;

    return Rectangle{
        (screenWidth - destWidth) / 2.0f,
        (screenHeight - destHeight) / 2.0f,
        destWidth,
        destHeight
    };
}

int main(void)
{
    srand(time(nullptr));

    // --- Заглушка модели персонажа (уже реализовано отдельно) ---------------
    // Создаём персонажа и печатаем характеристики в консоль — показывает, что
    // модель (Character/RoleStats) работает независимо от сцены ниже. Экран
    // создания персонажа и сам бой по кубикам будут отдельными следующими шагами.
    Character character = Character("test", RoleStats::Role::Rockerboy, rand() % 10 + 1);
    character.print();
    //--------------------------------------------------------------------------------------

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 720;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
    InitWindow(screenWidth, screenHeight, "Cyberpunk RED — Top-Down Prototype");

    // Переключает рабочую директорию на resources/, чтобы пути к текстурам ниже
    // ("Tiles/...", "Characters/...") работали что при запуске из IDE, что из
    // собранного .exe. Обязательно вызывается ПОСЛЕ InitWindow.
    SearchAndSetResourceDir("resources");

    // Сюда рисуется вся сцена в низком разрешении (см. пояснение в шапке файла).
    RenderTexture2D target = LoadRenderTexture(RENDER_WIDTH, RENDER_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT); // без сглаживания -> чёткие пиксели

    TileAtlas atlas;             // грузит все текстуры тайлов/персонажа один раз при старте

    // Карта задана ASCII-текстом (см. src/world/TestMaps.h) — так её форму видно
    // прямо в коде и легко редактировать руками, без вычисления координат стен.
    Grid grid(TestMaps::NeonAlley());

    TurnManager turnManager;     // счётчик ходов пошаговой игры

    // Все NPC сцены живут в NpcManager — main.cpp не объявляет их по одному
    // и не хранит имена переменных вроде "streetPunk"; сами данные (кто где
    // стоит, что говорит) лежат в src/world/NpcRoster.h. Чтобы добавить NPC —
    // правится только NpcRoster.h, здесь ничего менять не нужно.
    NpcManager npcManager(NpcRoster::NeonAlleyNpcs());

    Dialogue dialogue; // текущее состояние разговора: открыт ли, с кем, какая реплика

    // Шрифт с поддержкой кириллицы для HUD — встроенный шрифт raylib (тот, которым
    // рисует обычный DrawText) кириллицу не содержит вообще, поэтому русский текст
    // ниже рисуется через DrawTextEx этим шрифтом, а не через DrawText.
    Font uiFont = Fonts::LoadCyrillicFont("Fonts/ofont.ru_Hemico.ttf", 10);

    // Позиция игрока в клетках сетки (не в пикселях!). Экранные координаты
    // считаются из неё каждый кадр через World::GridToScreen.
    int playerGX = 1;
    int playerGY = 1;

    int playerFacing = FACING_SOUTH; // куда "смотрит" персонаж — влияет на строку спрайта
    int stepParity = 1;              // чередование кадров шага (1/2) для эффекта ходьбы
    double lastMoveTime = -1000.0;   // когда был последний шаг (для анимации стойки/шага)

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Какой NPC (если есть) сейчас рядом с игроком — нужно и для управления
        // (можно ли нажать E), и для отрисовки (иконка "можно поговорить"),
        // поэтому считаем один раз в начале кадра. NpcManager сам перебирает
        // всех NPC — здесь неважно, один их или десять.
        const Npc* interactableNpc = npcManager.FindInteractable(playerGX, playerGY);

        if (dialogue.IsOpen())
        {
            // Пока идёт диалог, обычное управление отключено полностью —
            // доступны только "дальше реплика" и "закрыть раньше времени".
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
                dialogue.Advance();
            else if (IsKeyPressed(KEY_ESCAPE))
                dialogue.Close();
        }
        else
        {
            // Пошаговое движение: используем IsKeyPressed (срабатывает один раз на
            // нажатие), а не IsKeyDown, — иначе персонаж бежал бы без остановки, пока
            // клавиша зажата. За кадр разрешаем только одно направление (без
            // "читерской" диагонали).
            int dx = 0, dy = 0;
            int pressedFacing = playerFacing;
            if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) { dx =  1; pressedFacing = FACING_EAST;  }
            else if (IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) { dx = -1; pressedFacing = FACING_WEST;  }
            else if (IsKeyPressed(KEY_DOWN)  || IsKeyPressed(KEY_S)) { dy =  1; pressedFacing = FACING_SOUTH; }
            else if (IsKeyPressed(KEY_UP)    || IsKeyPressed(KEY_W)) { dy =  -1; pressedFacing = FACING_NORTH; }

            if (dx != 0 || dy != 0)
            {
                // Поворачиваемся в сторону нажатия, даже если шаг окажется заблокирован
                // стеной, — так столкновение с препятствием выглядит осмысленно
                // (персонаж "смотрит" в стену, а не дёргается на месте вслепую).
                playerFacing = pressedFacing;

                int newX = playerGX + dx;
                int newY = playerGY + dy;

                // Клетка, занятая любым NPC, блокирует движение так же, как
                // стена — иначе игрок прошёл бы прямо сквозь него.
                bool blockedByNpc = npcManager.IsOccupied(newX, newY);

                if (grid.IsWalkable(newX, newY) && !blockedByNpc)
                {
                    playerGX = newX;
                    playerGY = newY;
                    turnManager.EndPlayerTurn(); // один успешный шаг = один завершённый ход

                    stepParity = (stepParity == 1) ? 2 : 1; // чередуем кадр шага для эффекта ходьбы
                    lastMoveTime = GetTime();
                }
                // Если клетка непроходима (стена или NPC) — ход не тратится.
            }

            // Взаимодействие: E рядом с NPC открывает диалог именно с тем NPC,
            // которого нашёл FindInteractable (не важно, какой он по счёту).
            if (interactableNpc && IsKeyPressed(KEY_E))
                dialogue.Open(*interactableNpc);
        }

        // Пока с момента шага прошло меньше STEP_POSE_DURATION — показываем кадр
        // шага, иначе возвращаемся к спокойной стойке (кадр 0).
        int playerFrame = (GetTime() - lastMoveTime < STEP_POSE_DURATION) ? stepParity : 0;
        //----------------------------------------------------------------------------------

        // Draw (в низкое разрешение)
        //----------------------------------------------------------------------------------
        BeginTextureMode(target); // всё между Begin/End рисуется в target, а не в окно
            ClearBackground(Color{ 12, 12, 20, 255 });

            // --- "Камера" сверху -----------------------------------------------------
            // Настоящей камеры нет: считаем, где на экране был бы центр клетки игрока
            // при нулевом смещении, и сдвигаем origin так, чтобы эта точка совпала
            // с центром экрана. В результате мир визуально "едет" под игроком.
            Vector2 playerTopLeft = World::GridToScreen(playerGX, playerGY, Vector2{ 0, 0 });
            Vector2 playerCenter  = {
                playerTopLeft.x + World::TILE_SIZE / 2.0f,
                playerTopLeft.y + World::TILE_SIZE / 2.0f
            };
            Vector2 origin = {
                RENDER_WIDTH  / 2.0f - playerCenter.x,
                RENDER_HEIGHT / 2.0f - playerCenter.y
            };

            // --- Отрисовка тайлов -----------------------------------------------------
            for (int y = 0; y < grid.GetHeight(); y++)
            {
                for (int x = 0; x < grid.GetWidth(); x++)
                {
                    Vector2 topLeft = World::GridToScreen(x, y, origin);

                    if (grid.IsWalkable(x, y))
                        TileRender::DrawFloor(topLeft, atlas.GetFloorTexture(x, y));
                    else
                        TileRender::DrawWall(topLeft, atlas.GetWallTexture());
                }
            }

            // --- NPC --------------------------------------------------------------
            // Один цикл рисует всех NPC разом — сколько бы их ни было в
            // NpcRoster.h, этот код не меняется. Рисуются до игрока (проще всего
            // считать, что NPC всегда "дальше" на этой маленькой сцене). Когда
            // актёров станет много, стоит сортировать по возрастанию gy, чтобы
            // те, кто ниже на экране, перекрывали тех, кто выше.
            for (const Npc& npc : npcManager.All())
            {
                Vector2 npcScreenTopLeft = World::GridToScreen(npc.gx, npc.gy, origin);
                Vector2 npcFeetPos = {
                    npcScreenTopLeft.x + World::TILE_SIZE / 2.0f,
                    npcScreenTopLeft.y + World::TILE_SIZE
                };
                // NPC использует ту же текстуру, что и игрок (в resources пока
                // только один спрайт персонажа) — розоватый tint нужен просто
                // чтобы отличать их на глаз. Кадр всегда "стойка лицом на юг".
                Rectangle npcFrameRect = atlas.GetPlayerFrameRect(FACING_SOUTH, 0);
                TileRender::DrawPlayer(npcFeetPos, atlas.GetPlayerTexture(), npcFrameRect, Color{ 255, 170, 210, 255 });

                // Иконка "можно поговорить" — только над тем NPC, к которому
                // игрок реально стоит вплотную, и только пока диалог не открыт.
                if (&npc == interactableNpc && !dialogue.IsOpen())
                {
                    Vector2 hintPos = { npcFeetPos.x, npcFeetPos.y - 48 };
                    TileRender::DrawInteractHint(hintPos, atlas.GetSpeechBubbleTexture());
                }
            }

            // --- Игрок ------------------------------------------------------------
            Rectangle frameRect = atlas.GetPlayerFrameRect(playerFacing, playerFrame);
            Vector2 playerScreenTopLeft = World::GridToScreen(playerGX, playerGY, origin);
            Vector2 feetPos = {
                playerScreenTopLeft.x + World::TILE_SIZE / 2.0f,
                playerScreenTopLeft.y + World::TILE_SIZE
            };
            TileRender::DrawPlayer(feetPos, atlas.GetPlayerTexture(), frameRect);

            // HUD рисуется поверх сцены последним, прямо в той же низкоразрешённой текстуре.
            // DrawTextEx + uiFont вместо DrawText — см. пояснение у объявления uiFont выше.
            DrawTextEx(uiFont, TextFormat("Ход: %d", turnManager.GetTurnNumber()), Vector2{ 6, 6 }, 10, 1, RAYWHITE);

            if (dialogue.IsOpen())
            {
                // Само окно диалога перекрывает нижнюю подсказку по управлению —
                // в этот момент она не нужна, место занято текстом реплики.
                DialogueBox::Draw(dialogue, uiFont, RENDER_WIDTH, RENDER_HEIGHT);
            }
            else
            {
                // Контекстная подсказка снизу: рядом с NPC — как заговорить,
                // иначе — обычное напоминание про управление.
                const char* bottomHint = interactableNpc
                    ? "E - поговорить"
                    : "WASD / стрелки - движение по тайлам";
                DrawTextEx(uiFont, bottomHint, Vector2{ 6, RENDER_HEIGHT - 16 }, 10, 1, Fade(RAYWHITE, 0.7f));
            }
        EndTextureMode();
        //----------------------------------------------------------------------------------

        // Draw (окно — масштабируем рендер-текстуру с точечной фильтрацией)
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(BLACK); // цвет полос letterbox, если пропорции окна не совпадают

            Rectangle src = { 0, 0, (float)target.texture.width, -(float)target.texture.height };
            Rectangle dst = GetScaledDestRect(GetScreenWidth(), GetScreenHeight());
            DrawTexturePro(target.texture, src, dst, Vector2{ 0, 0 }, 0.0f, WHITE);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadFont(uiFont);
    UnloadRenderTexture(target); // освобождаем GPU-ресурсы рендер-текстуры (atlas выгружается сам, в деструкторе)
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
