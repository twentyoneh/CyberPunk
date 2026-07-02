#pragma once
#include "CombatUnit.h"
#include "../game/EnemyManager.h"
#include <vector>
#include <string>

// Отдельная тактическая сцена боя — своя маленькая арена (в духе Heroes of
// Might & Magic: игрок и враги стоят на клетках отдельной сетки, ходят по
// очереди по инициативе), а не бой прямо на карте исследования. main.cpp
// создаёт Battle, когда игрок толкается во врага на карте, и вызывает
// Update() каждый кадр, пока GetOutcome() не перестанет быть Ongoing (см.
// BattleRender.h для отрисовки и main.cpp для переключения сцен).
//
// Упрощено по сравнению с настольными правилами Cyberpunk RED: инициатива —
// один бросок d10+REF на весь бой (не перебрасывается каждый ход), ход — это
// одно действие (шаг ИЛИ атака), урон и попадание считает CombatMath.
class Battle
{
public:
    enum class Outcome { Ongoing, Victory, Defeat };

    // player      — персонаж игрока (Battle не владеет, только ссылается).
    // enemies     — менеджер врагов сцены исследования; сюда же пишется
    //               результат победы (см. MarkDefeatedEnemies).
    // enemyIndices — какие враги (по индексу в enemies) участвуют в этом бою.
    Battle(Character& player, EnemyManager& enemies, const std::vector<int>& enemyIndices,
           int arenaWidth = 10, int arenaHeight = 6);

    // Попытка игрока шагнуть на арене. Ничего не делает, если сейчас не ход
    // игрока, бой уже закончился, клетка вне арены или занята. При успехе
    // шаг тратит ход игрока (передаёт ход дальше по инициативе).
    void TryMovePlayer(int dx, int dy);

    // Атака ближайшего (вплотную, Манхэттен == 1) живого врага. Ничего не
    // делает, если сейчас не ход игрока или рядом никого нет.
    void TryPlayerAttack();

    // Вызывать каждый кадр из main.cpp. Пока не ход игрока (враг ходит) —
    // сама выбирает и выполняет действие ИИ по истечении небольшой паузы
    // (см. enemyActionTimer), чтобы вражеские ходы не мелькали за один кадр.
    void Update(float dt);

    bool IsPlayerTurn() const { return outcome == Outcome::Ongoing && units[currentUnitIndex].isPlayer; }
    Outcome GetOutcome() const { return outcome; }

    // Индекс юнита, чей сейчас ход — нужен только для подсветки в BattleRender,
    // на саму логику боя не влияет.
    int GetCurrentUnitIndex() const { return currentUnitIndex; }

    const std::vector<CombatUnit>& GetUnits() const { return units; }
    const std::vector<std::string>& GetLog() const { return log; }

    int GetArenaWidth() const { return arenaWidth; }
    int GetArenaHeight() const { return arenaHeight; }

    // После победы main.cpp вызывает это, чтобы пометить всех побеждённых
    // врагов в EnemyManager флагом defeated (а не просто "мёртвый Character",
    // который IsOccupied/FindEngageableIndex не проверяют).
    void MarkDefeatedEnemies();

private:
    int arenaWidth;
    int arenaHeight;

    std::vector<CombatUnit> units; // units[0] — всегда игрок, см. конструктор
    int currentUnitIndex = 0;
    Outcome outcome = Outcome::Ongoing;

    EnemyManager* enemyManager;

    std::vector<std::string> log;
    static constexpr size_t MAX_LOG_LINES = 6;

    double enemyActionTimer = 0.0;
    static constexpr double ENEMY_ACTION_DELAY = 0.6; // сек. паузы перед ходом врага

    bool IsCellOccupied(int x, int y, const CombatUnit* ignore) const;
    CombatUnit* GetPlayerUnit();
    CombatUnit* FindAdjacentLivingEnemy(const CombatUnit& from);

    void AddLog(const std::string& line);
    void CheckOutcome();
    void AdvanceTurn(); // передаёт ход следующему живому юниту по очереди инициативы
    void DoEnemyAction(CombatUnit& unit);
    void ResolveAttackBetween(CombatUnit& attacker, CombatUnit& defender);
};
