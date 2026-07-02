#include "Battle.h"
#include "CombatMath.h"
#include "Dice.h"
#include <algorithm>
#include <cstdlib>

Battle::Battle(Character& player, EnemyManager& enemies, const std::vector<int>& enemyIndices,
               int arenaWidth, int arenaHeight)
    : arenaWidth(arenaWidth)
    , arenaHeight(arenaHeight)
    , enemyManager(&enemies)
{
    // Игрок всегда стоит слева, враги — справа, по вертикали друг под другом.
    // Настоящей расстановки/укрытий пока нет — это самый простой вариант
    // арены, чтобы отработать сам цикл боя (ход/атака/инициатива).
    CombatUnit playerUnit;
    playerUnit.ax = 1;
    playerUnit.ay = arenaHeight / 2;
    playerUnit.character = &player;
    playerUnit.isPlayer = true;
    playerUnit.enemyIndex = -1;
    units.push_back(playerUnit);

    int startY = arenaHeight / 2 - (int)enemyIndices.size() / 2;
    for (size_t i = 0; i < enemyIndices.size(); i++)
    {
        CombatUnit enemyUnit;
        enemyUnit.ax = arenaWidth - 2;

        int y = startY + (int)i;
        if (y < 0) y = 0;
        if (y >= arenaHeight) y = arenaHeight - 1;
        enemyUnit.ay = y;

        enemyUnit.character = &enemies.Get(enemyIndices[i]).character;
        enemyUnit.isPlayer = false;
        enemyUnit.enemyIndex = enemyIndices[i];
        units.push_back(enemyUnit);
    }

    // Инициатива: один бросок d10 + REF на весь бой — проще, чем перебрасывать
    // каждый ход, как в настольных правилах, но достаточно, чтобы порядок
    // ходов не был всегда одинаковым.
    for (CombatUnit& u : units)
        u.initiative = Dice::RollD10() + u.character->getStats().REF;

    // stable_sort: при равной инициативе порядок не меняется — игрок (первым
    // оказавшийся в векторе) выигрывает ничьи, что не даёт врагам ходить
    // "раньше" появления игрока в списке без причины.
    std::stable_sort(units.begin(), units.end(), [](const CombatUnit& a, const CombatUnit& b) {
        return a.initiative > b.initiative;
    });

    currentUnitIndex = 0;
    enemyActionTimer = ENEMY_ACTION_DELAY;

    AddLog("Бой начался!");
    CheckOutcome();
}

void Battle::TryMovePlayer(int dx, int dy)
{
    if (!IsPlayerTurn()) return;

    CombatUnit* player = GetPlayerUnit();
    int nx = player->ax + dx;
    int ny = player->ay + dy;

    if (nx < 0 || nx >= arenaWidth || ny < 0 || ny >= arenaHeight) return;
    if (IsCellOccupied(nx, ny, player)) return;

    player->ax = nx;
    player->ay = ny;
    AddLog(player->character->getName() + " делает шаг.");
    AdvanceTurn();
}

void Battle::TryPlayerAttack()
{
    if (!IsPlayerTurn()) return;

    CombatUnit* player = GetPlayerUnit();
    CombatUnit* target = FindAdjacentLivingEnemy(*player);
    if (!target) return;

    ResolveAttackBetween(*player, *target);
    CheckOutcome();
    if (outcome == Outcome::Ongoing)
        AdvanceTurn();
}

void Battle::Update(float dt)
{
    if (outcome != Outcome::Ongoing) return;
    if (IsPlayerTurn()) return; // ждём TryMovePlayer/TryPlayerAttack из main.cpp

    enemyActionTimer -= dt;
    if (enemyActionTimer > 0.0) return;

    CombatUnit& current = units[currentUnitIndex];
    if (current.character->isAlive())
        DoEnemyAction(current);

    CheckOutcome();
    if (outcome == Outcome::Ongoing)
        AdvanceTurn();

    enemyActionTimer = ENEMY_ACTION_DELAY;
}

void Battle::MarkDefeatedEnemies()
{
    for (const CombatUnit& u : units)
    {
        if (u.isPlayer) continue;
        if (!u.character->isAlive())
            enemyManager->MarkDefeated(u.enemyIndex);
    }
}

bool Battle::IsCellOccupied(int x, int y, const CombatUnit* ignore) const
{
    for (const CombatUnit& u : units)
    {
        if (&u == ignore) continue;
        if (!u.character->isAlive()) continue;
        if (u.ax == x && u.ay == y) return true;
    }
    return false;
}

CombatUnit* Battle::GetPlayerUnit()
{
    for (CombatUnit& u : units)
        if (u.isPlayer) return &u;
    return nullptr;
}

CombatUnit* Battle::FindAdjacentLivingEnemy(const CombatUnit& from)
{
    for (CombatUnit& u : units)
    {
        if (u.isPlayer) continue;
        if (!u.character->isAlive()) continue;

        int distX = std::abs(from.ax - u.ax);
        int distY = std::abs(from.ay - u.ay);
        if (distX + distY == 1) return &u;
    }
    return nullptr;
}

void Battle::AddLog(const std::string& line)
{
    log.push_back(line);
    while (log.size() > MAX_LOG_LINES)
        log.erase(log.begin());
}

void Battle::CheckOutcome()
{
    CombatUnit* player = GetPlayerUnit();
    if (!player || !player->character->isAlive())
    {
        outcome = Outcome::Defeat;
        return;
    }

    bool anyEnemyAlive = false;
    for (const CombatUnit& u : units)
    {
        if (!u.isPlayer && u.character->isAlive())
        {
            anyEnemyAlive = true;
            break;
        }
    }

    if (!anyEnemyAlive)
        outcome = Outcome::Victory;
}

void Battle::AdvanceTurn()
{
    if (outcome != Outcome::Ongoing) return;

    for (size_t attempts = 0; attempts < units.size(); attempts++)
    {
        currentUnitIndex = (currentUnitIndex + 1) % units.size();
        if (units[currentUnitIndex].character->isAlive())
            return;
    }
    // Если сюда дошли — все остальные юниты мертвы; CheckOutcome() выше
    // должен был закончить бой раньше, чем мы окажемся в этой ситуации.
}

void Battle::DoEnemyAction(CombatUnit& unit)
{
    CombatUnit* player = GetPlayerUnit();
    if (!player || !player->character->isAlive()) return;

    int distX = player->ax - unit.ax;
    int distY = player->ay - unit.ay;

    if (std::abs(distX) + std::abs(distY) == 1)
    {
        ResolveAttackBetween(unit, *player);
        return;
    }

    int stepX = (distX > 0) - (distX < 0);
    int stepY = (distY > 0) - (distY < 0);
    bool moveOnX = std::abs(distX) >= std::abs(distY);

    auto tryMove = [&](int dx, int dy) -> bool {
        int nx = unit.ax + dx;
        int ny = unit.ay + dy;
        if (nx < 0 || nx >= arenaWidth || ny < 0 || ny >= arenaHeight) return false;
        if (IsCellOccupied(nx, ny, &unit)) return false;
        unit.ax = nx;
        unit.ay = ny;
        return true;
    };

    bool moved = false;
    if (moveOnX && stepX != 0)
        moved = tryMove(stepX, 0);
    else if (!moveOnX && stepY != 0)
        moved = tryMove(0, stepY);

    if (!moved)
    {
        // Основная ось заблокирована (например, другим врагом) — пробуем
        // сдвинуться по второй оси, чтобы враг не "залипал" на месте.
        if (moveOnX && stepY != 0)
            moved = tryMove(0, stepY);
        else if (!moveOnX && stepX != 0)
            moved = tryMove(stepX, 0);
    }

    if (moved)
        AddLog(unit.character->getName() + " приближается.");
}

void Battle::ResolveAttackBetween(CombatUnit& attacker, CombatUnit& defender)
{
    if (attacker.character->getInventory().weapons.empty())
    {
        AddLog(attacker.character->getName() + " безоружен(а) и пропускает ход.");
        return;
    }

    const Weapon& weapon = attacker.character->getInventory().weapons[0];
    CombatMath::AttackOutcome result = CombatMath::ResolveAttack(*attacker.character, *defender.character, weapon);

    if (result.hit)
    {
        defender.character->takeDamage(result.damage);
        AddLog(attacker.character->getName() + " попадает по " + defender.character->getName() +
               " (урон " + std::to_string(result.damage) + ", HP " +
               std::to_string(defender.character->getCurrentHP()) + "/" +
               std::to_string(defender.character->getMaxHP()) + ")");

        if (!defender.character->isAlive())
            AddLog(defender.character->getName() + " повержен(а)!");
    }
    else
    {
        AddLog(attacker.character->getName() + " атакует " + defender.character->getName() + ", но промахивается.");
    }
}
