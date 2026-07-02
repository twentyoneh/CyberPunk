#pragma once
#include "../character/Character.h"
#include "../character/Weapon.h"

// Расчёт одной атаки. Сильно упрощено по сравнению с настольным Cyberpunk RED
// (нет прицельных выстрелов, критов, доп. эффектов оружия и т.д.) — как и
// просили: всё решается одним встречным броском кубика плюс статы/навыки.
namespace CombatMath
{
    struct AttackOutcome
    {
        bool hit;
        int  attackRoll;   // бросок атакующего (d10 + навык + стат) — для лога боя
        int  defenseRoll;  // встречный бросок защищающегося (d10 + Уклонение + REF)
        int  damage;       // 0, если промах
    };

    // Атакующий и защищающийся бросают каждый d10 + свои значения; у кого
    // больше — тот и выиграл встречный бросок. При ничьей побеждает
    // защищающийся (иначе на равных статах будут "гарантированные" попадания).
    // При попадании урон = сумма кубиков d6 оружия (weapon.damage — сколько
    // кубиков, см. Weapon.h) минус СП брони корпуса защищающегося.
    AttackOutcome ResolveAttack(const Character& attacker, const Character& defender, const Weapon& weapon);
}
