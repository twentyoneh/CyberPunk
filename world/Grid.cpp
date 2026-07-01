#include "Grid.h"
#include <cstddef>

// Генерация тестовой карты. Пока это просто: рамка стен по периметру
// (чтобы игрок не мог уйти за пределы сцены) + один короткий отрезок стены
// внутри (obstacle) — чисто для проверки, что движение реально блокируется
// препятствиями, а не только границами карты.
Grid::Grid(int width, int height)
    : width(width), height(height), tiles(width * height)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Tile& t = tiles[y * width + x]; // индекс в плоском массиве = y * ширина + x

            bool border   = (x == 0 || y == 0 || x == width - 1 || y == height - 1);
            bool obstacle = (x == 4 && y >= 2 && y <= 4); // тестовая стена-препятствие для проверки движения

            t.type = (border || obstacle) ? TileType::Wall : TileType::Floor;
        }
    }
}

// Карта из ASCII-текста. width берём по длине первой строки, height — по
// количеству строк. '#' -> стена, любой другой символ (включая пробел) -> пол —
// специально не завязываемся только на '.', чтобы опечатка в символе пола
// не превращала клетку в стену.
Grid::Grid(const std::vector<std::string>& rows)
    : width(rows.empty() ? 0 : static_cast<int>(rows[0].size()))
    , height(static_cast<int>(rows.size()))
    , tiles(static_cast<size_t>(width) * static_cast<size_t>(height))
{
    for (int y = 0; y < height; y++)
    {
        const std::string& row = rows[y];

        for (int x = 0; x < width; x++)
        {
            // Если какая-то строка карты короче остальных (опечатка при наборе),
            // не вылезаем за её границы, а просто считаем недостающий хвост стеной.
            char c = (x < static_cast<int>(row.size())) ? row[x] : '#';
            switch (c)
            {
                case '#': tiles[y * width + x].type = TileType::Wall;  break;
                // добавить другие символы для разных типов тайлов (например, 'T' -> TileType::Trap) можно здесь
                default:  tiles[y * width + x].type = TileType::Floor; break;
            }
        }
    }
}

bool Grid::InBounds(int x, int y) const
{
    return x >= 0 && y >= 0 && x < width && y < height;
}

bool Grid::IsWalkable(int x, int y) const
{
    // Порядок важен: сначала проверяем границы, и только если внутри карты —
    // лезем в массив tiles. Иначе при выходе за границы было бы обращение
    // к памяти по неверному индексу.
    return InBounds(x, y) && tiles[y * width + x].IsWalkable();
}

const Tile& Grid::At(int x, int y) const
{
    return tiles[y * width + x];
}
