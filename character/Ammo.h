#pragma once
#include <string>

enum class AmmoType {
    None,            // для холодного оружия
    LightPistol,     // лёгкий пистолет
    MediumPistol,    // средний пистолет
    HeavyPistol,     // тяжелый пистолет (Т)
    VeryHeavyPistol, // очень тяжелый пистолет (ОТ)
    Rifle,           // штурмовая винтовка
    Shotgun,         // дробовик
};

enum class AmmoVariant {
    Basic,       // обычные
    ArmorPiercing,   // бронебойные
    HollowPoint,     // экспансивные
    Incendiary,      // зажигательные
};

inline const char* ammoTypeName(AmmoType t) {
    switch (t) {
        case AmmoType::LightPistol:     return "Лёгкий пистолет";
        case AmmoType::MediumPistol:    return "Средний пистолет";
        case AmmoType::HeavyPistol:     return "Тяжелый пистолет";
        case AmmoType::VeryHeavyPistol: return "Очень тяжелый пистолет";
        case AmmoType::Rifle:           return "Винтовка";
        case AmmoType::Shotgun:         return "Дробовик";
        default:                        return "-";
    }
}
