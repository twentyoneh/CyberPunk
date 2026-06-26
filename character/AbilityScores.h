#pragma once
#include <iostream>

struct AbilityScores {
    int INT_  = 0;  // Интеллект
    int REF   = 0;  // Рефлексы
    int DEX   = 0;  // Ловкость
    int TECH  = 0;  // Техника
    int CHAR  = 0;  // Харизма
    int WILL  = 0;  // Воля
    int LUCK  = 0;  // Удача
    int MOVE  = 0;  // Скорость
    int BODY  = 0;  // Телосложение
    int EMP   = 0;  // Эмпатия

    void print() const {
        std::cout << "ИНТ:  " << INT_  << "\n";
        std::cout << "РЕА:  " << REF   << "\n";
        std::cout << "ЛВК:  " << DEX   << "\n";
        std::cout << "ТЕХ:  " << TECH  << "\n";
        std::cout << "ХАР:  " << CHAR  << "\n";
        std::cout << "ВОЛЯ: " << WILL  << "\n";
        std::cout << "УДЧ:  " << LUCK  << "\n";
        std::cout << "СКО:  " << MOVE  << "\n";
        std::cout << "ТЕЛ:  " << BODY  << "\n";
        std::cout << "ЭМП:  " << EMP   << "\n";
        std::cout << "ЗДОРОВЬЕ: " << HP    << "\n";
    }
};
