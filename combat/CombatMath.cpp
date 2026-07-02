#include "CombatMath.h"
#include "Dice.h"

namespace
{
    // Сильно упрощённая привязка оружия к навыку: в реальных правилах Cyberpunk
    // RED у винтовок/дробовиков/автоматов свои отдельные навыки — здесь всего
    // два варианта: "в руках оружие ближнего боя" или "во всём остальном
    // случае пистолеты". Если у роли нет такого навыка, getSkillLevel просто
    // вернёт 0 — это ожидаемо, не баг (см. Character::getSkillLevel).
    constexpr const char* MELEE_SKILL   = "Оружие ближнего боя";
    constexpr const char* RANGED_SKILL  = "Пистолеты";
    constexpr const char* EVASION_SKILL = "Уклонение";

    int GetBodyArmorStoppingPower(const Character& defender)
    {
        // Берём первую броню корпуса — не различаем конкретную зону попадания
        // (голова/тело), как в полной таблице Cyberpunk RED.
        for (const Armor& armor : defender.getInventory().armor)
        {
            if (armor.location == ArmorLocation::Body)
                return armor.stoppingPower;
        }
        return 0;
    }
}

namespace CombatMath
{
    AttackOutcome ResolveAttack(const Character& attacker, const Character& defender, const Weapon& weapon)
    {
        bool isMelee = (weapon.ammoType == AmmoType::None);

        const char* attackSkillName = isMelee ? MELEE_SKILL : RANGED_SKILL;
        int attackStat = isMelee ? attacker.getStats().DEX : attacker.getStats().REF;

        int attackRoll  = Dice::RollD10() + attacker.getSkillLevel(attackSkillName) + attackStat;
        int defenseRoll = Dice::RollD10() + defender.getSkillLevel(EVASION_SKILL) + defender.getStats().REF;

        AttackOutcome outcome{};
        outcome.attackRoll  = attackRoll;
        outcome.defenseRoll = defenseRoll;
        outcome.hit         = attackRoll > defenseRoll; // ничья = промах, см. .h
        outcome.damage      = 0;

        if (outcome.hit)
        {
            int rawDamage = Dice::RollNd6(weapon.damage);
            int armorSP   = GetBodyArmorStoppingPower(defender);

            int finalDamage = rawDamage - armorSP;
            if (finalDamage < 0) finalDamage = 0;

            outcome.damage = finalDamage;
        }

        return outcome;
    }
}
