#include "RoleEquipment.h"

int RoleEquipment::getChoice(const std::vector<int>& choices, int index, int defaultVal) {
    if (index < (int)choices.size()) return choices[index];
    return defaultVal;
}

// --- Оружие ---
static Weapon makeVHP()        { return {"Очень тяжелый пистолет",     AmmoType::VeryHeavyPistol, 4, 8,  8 }; }
static Weapon makeHP()         { return {"Тяжелый пистолет",           AmmoType::HeavyPistol,     3, 8,  8 }; }
static Weapon makeMP()         { return {"Средний пистолет",            AmmoType::MediumPistol,    2, 12, 12}; }
static Weapon makeRifle()      { return {"Штурмовая винтовка",          AmmoType::Rifle,           5, 25, 25}; }
static Weapon makeShotgun()    { return {"Дробовик",                    AmmoType::Shotgun,         5, 4,  4 }; }
static Weapon makeHeavyMelee() { return {"Тяжелое оружие ближнего боя", AmmoType::None,            3, 0,  0 }; }
static Weapon makeLightMelee() { return {"Лёгкое оружие ближнего боя",  AmmoType::None,            1, 0,  0 }; }

// --- Броня ---
static Armor makeBodyArmor()    { return {"Легкий бронекостюм",      ArmorLocation::Body, 11}; }
static Armor makeHeadArmor()    { return {"Легкая броня для головы",  ArmorLocation::Head, 11}; }
static Armor makeBulletShield() { return {"Пуленепробиваемый щит",   ArmorLocation::Body, 10}; }

Inventory RoleEquipment::getInventory(Role role, const std::vector<int>& choices) {
    Inventory inv;

    switch (role) {

    // -------------------------------------------------------
    case Role::Rockerboy:
        // Оружие/Броня
        // Выбор 0: Тяжелое оружие ближ. боя [0] или Светошумовая граната [1]
        inv.weapons.push_back(makeVHP());
        inv.addAmmo(AmmoType::VeryHeavyPistol, 50);
        if (getChoice(choices, 0) == 0)
            inv.weapons.push_back(makeHeavyMelee());
        else
            inv.grenades.push_back({"Светошумовая граната", GrenadeType::Flashbang, 1});
        inv.grenades.push_back({"Граната со слезоточивым газом", GrenadeType::Teargas, 2});
        inv.armor.push_back(makeBodyArmor());
        inv.armor.push_back(makeHeadArmor());
        // Снаряжение
        // Выбор 1: Электрогитара [0] или Детектор жучков [1]
        inv.gear = {
            {"Агент"},
            {"Светящаяся краска", 5},
            {"Карманный усилитель (AMP)"},
            {"Радио сканер/музыкальный проигрыватель"},
            {"Видеокамера"},
            {"Компьютер"},
            {getChoice(choices, 1) == 0 ? "Электрогитара" : "Детектор жучков"},
        };
        // Одежда
        inv.clothing = {
            {ClothingStyle::GenericChic,   {{"куртка"}, {"украшения", 3}, {"топ", 4}}},
            {ClothingStyle::Leisurewear,   {{"ювелирика"}, {"очки"}, {"обувь"}}},
            {ClothingStyle::Urbanflash,    {{"штаны"}, {"верх"}}},
        };
        break;

    // -------------------------------------------------------
    case Role::Solo:
        // Оружие/Броня
        inv.weapons.push_back(makeRifle());
        inv.weapons.push_back(makeVHP());
        inv.addAmmo(AmmoType::HeavyPistol, 30);
        inv.addAmmo(AmmoType::Rifle, 70);
        inv.armor.push_back(makeBulletShield());
        inv.armor.push_back(makeBodyArmor());
        inv.armor.push_back(makeHeadArmor());
        // Снаряжение
        inv.gear = {
            {"Агент"},
        };
        // Одежда
        inv.clothing = {
            {ClothingStyle::Leisurewear, {{"обувь", 2}, {"куртка", 3}, {"очки"}, {"штаны", 2}, {"верх", 2}}},
        };
        break;

    // -------------------------------------------------------
    case Role::Netrunner:
        // Оружие/Броня
        inv.weapons.push_back(makeVHP());
        inv.addAmmo(AmmoType::VeryHeavyPistol, 30);
        inv.armor.push_back(makeBodyArmor());
        inv.armor.push_back(makeHeadArmor());
        // Снаряжение (программы кибердеки — пока как GearItem)
        inv.gear = {
            {"Агент"},
            {"Кибердека (7 слотов)"},
            {"Очки виртуальности"},
            {"Программа: Доспехи"},
            {"Программа: Найдёмся"},
            {"Программа: Стиратель"},
            {"Программа: Меч",    2},
            {"Программа: Висп"},
            {"Программа: Червь"},
        };
        // Одежда
        inv.clothing = {
            {ClothingStyle::GenericChic, {{"верх", 10}}},
            {ClothingStyle::Leisurewear, {{"обувь", 2}, {"ювелирика"}, {"штаны", 2}}},
            {ClothingStyle::Urbanflash,  {{"куртка"}}},
        };
        break;

    // -------------------------------------------------------
    case Role::Tech:
        // Оружие/Броня
        inv.weapons.push_back(makeShotgun());
        inv.weapons.push_back(makeRifle());
        inv.addAmmo(AmmoType::Rifle, 100);
        inv.addAmmo(AmmoType::Shotgun, 100);
        inv.grenades.push_back({"Светошумовая граната", GrenadeType::Flashbang, 1});
        inv.armor.push_back(makeBodyArmor());
        inv.armor.push_back(makeHeadArmor());
        // Снаряжение
        inv.gear = {
            {"Агент"},
            {"Маска-противогаз"},
            {"Одноразовый телефон"},
            {"Клейкая лента", 5},
            {"Фонарик"},
            {"Сигнальная ракета", 6},
            {"Сумка с инструментами"},
        };
        // Одежда
        inv.clothing = {
            {ClothingStyle::GenericChic, {{"штаны", 8}, {"верх", 10}}},
            {ClothingStyle::Leisurewear, {{"обувь", 2}}},
        };
        break;

    // -------------------------------------------------------
    case Role::Medtech:
        // Оружие/Броня
        inv.weapons.push_back(makeShotgun());
        inv.weapons.push_back(makeRifle());
        inv.addAmmo(AmmoType::Rifle, 100);
        inv.addAmmo(AmmoType::Shotgun, 100);
        // TODO: зажигательные патроны x10 (Rifle + Shotgun) — требует AmmoVariant
        inv.grenades.push_back({"Дымовая граната", GrenadeType::Smoke, 2});
        inv.armor.push_back(makeBodyArmor());
        inv.armor.push_back(makeHeadArmor());
        inv.armor.push_back(makeBulletShield());
        // Снаряжение
        inv.gear = {
            {"Агент"},
            {"Аэрогиппо"},
            {"Наручники"},
            {"Фонарик"},
            {"Светящаяся краска"},
            {"Сумка медтеха"},
        };
        // Одежда
        inv.clothing = {
            {ClothingStyle::GenericChic, {{"куртка", 3}}},
            {ClothingStyle::Leisurewear, {{"обувь"}, {"штаны", 3}, {"верх", 5}}},
        };
        break;

    // -------------------------------------------------------
    case Role::Media:
        // Оружие/Броня
        inv.weapons.push_back(makeVHP());
        inv.addAmmo(AmmoType::VeryHeavyPistol, 50);
        inv.armor.push_back(makeBodyArmor());
        inv.armor.push_back(makeHeadArmor());
        // Снаряжение
        inv.gear = {
            {"Агент"},
            {"Диктофон"},
            {"Бинокль"},
            {"Одноразовый телефон", 2},
            {"Пистолет-крюк"},
            {"Фонарик"},
            {"Компьютер"},
            {"Радио сканер/музыкальный проигрыватель"},
            {"Шифратор/Дешифратор"},
            {"Видеокамера"},
        };
        // Одежда
        inv.clothing = {
            {ClothingStyle::GenericChic, {{"обувь"}, {"штаны"}, {"верх"}}},
            {ClothingStyle::Leisurewear, {{"куртка"}}},
            {ClothingStyle::Urbanflash,  {{"очки"}}},
        };
        break;

    // -------------------------------------------------------
    case Role::Lawman:
        // Оружие/Броня
        // Выбор 0: Пуленепробиваемый щит [0] или Дымовая граната x2 [1]
        inv.weapons.push_back(makeShotgun());
        inv.weapons.push_back(makeRifle());
        inv.weapons.push_back(makeMP());
        inv.addAmmo(AmmoType::Rifle, 100);
        inv.addAmmo(AmmoType::Shotgun, 100);
        inv.addAmmo(AmmoType::MediumPistol, 30);
        if (getChoice(choices, 0) == 0)
            inv.armor.push_back(makeBulletShield());
        else
            inv.grenades.push_back({"Дымовая граната", GrenadeType::Smoke, 2});
        inv.armor.push_back(makeBodyArmor());
        inv.armor.push_back(makeHeadArmor());
        // Снаряжение
        inv.gear = {
            {"Агент"},
            {"Фонарик"},
            {"Наручники", 2},
            {"Рация"},
            {"Сигнальная ракета", 10},
        };
        // Одежда
        inv.clothing = {
            {ClothingStyle::GenericChic, {{"куртка"}, {"штаны", 2}, {"верх", 3}}},
            {ClothingStyle::Leisurewear, {{"обувь", 2}, {"куртка", 2}, {"штаны", 2}, {"очки"}, {"верх", 2}}},
        };
        break;

    // -------------------------------------------------------
    case Role::Corp:
        // Оружие/Броня
        inv.weapons.push_back(makeVHP());
        inv.addAmmo(AmmoType::VeryHeavyPistol, 50);
        inv.armor.push_back(makeBodyArmor());
        inv.armor.push_back(makeHeadArmor());
        // Снаряжение
        inv.gear = {
            {"Рация", 4},
            {"Шифратор/Дешифратор"},
        };
        // Одежда
        inv.clothing = {
            {ClothingStyle::Businesswear, {{"обувь"}, {"куртка"}, {"штаны"}, {"очки"}, {"верх"}, {"ювелирные изделия", 2}}},
        };
        break;

    // -------------------------------------------------------
    case Role::Fixer:
        // Оружие/Броня
        // Выбор 0: первый пистолет — ТП [0] или ОТП [1]
        // Выбор 1: второй пистолет — ТП [0] или ОТП [1]
        inv.weapons.push_back(getChoice(choices, 0) == 0 ? makeHP() : makeVHP());
        inv.weapons.push_back(getChoice(choices, 1) == 0 ? makeHP() : makeVHP());
        inv.weapons.push_back(makeLightMelee());
        inv.addAmmo(AmmoType::HeavyPistol, 100);
        inv.addAmmo(AmmoType::VeryHeavyPistol, 100);
        inv.armor.push_back(makeBodyArmor());
        inv.armor.push_back(makeHeadArmor());
        // Снаряжение
        inv.gear = {
            {"Агент"},
            {"Детектор жучков"},
            {"Компьютер"},
            {"Одноразовый телефон", 2},
        };
        // Одежда
        inv.clothing = {
            {ClothingStyle::GenericChic, {{"контактные линзы"}, {"ювелирика"}}},
            {ClothingStyle::Urbanflash,  {{"обувь"}, {"куртка"}, {"штаны"}, {"верх"}}},
            {ClothingStyle::Leisurewear, {{"очки"}}},
        };
        break;

    // -------------------------------------------------------
    case Role::Nomad:
        // Оружие/Броня
        // Выбор 0: пистолет — ТП [0] или ОТП [1]
        // Выбор 1: второй слот — Тяжелое оружие ближ. боя [0] или ТП [1]
        inv.weapons.push_back(getChoice(choices, 0) == 0 ? makeHP() : makeVHP());
        inv.addAmmo(AmmoType::HeavyPistol, 100);
        inv.addAmmo(AmmoType::VeryHeavyPistol, 100);
        if (getChoice(choices, 1) == 0)
            inv.weapons.push_back(makeHeavyMelee());
        else
            inv.weapons.push_back(makeHP());
        inv.armor.push_back(makeBodyArmor());
        inv.armor.push_back(makeHeadArmor());
        // Снаряжение
        inv.gear = {
            {"Агент"},
            {"Маска-противогаз"},
            {"Скотч"},
            {"Фонарик"},
            {"Пистолет-крюк"},
            {"Складная кровать и спальный мешок"},
            {"Сумка медтеха"},
            {"Рация", 2},
            {"Верёвка"},
            {"Гаечный ключ (или любой другой инструмент)"},
            {"Палатка и оборудование для кемпинга"},
        };
        // Одежда
        inv.clothing = {
            {ClothingStyle::Bohemian,      {{"ювелирика"}}},
            {ClothingStyle::NomadLeathers, {{"верх", 4}, {"штаны", 2}, {"обувь", 2}, {"куртка"}, {"шляпа"}}},
        };
        break;

    default:
        break;
    }

    return inv;
}
