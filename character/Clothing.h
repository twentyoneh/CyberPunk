#pragma once
#include <string>
#include <vector>

enum class ClothingStyle {
    GenericChic,
    Leisurewear,
    Urbanflash,
    Businesswear,
    Bohemian,
    NomadLeathers,
};

inline const char* clothingStyleName(ClothingStyle s) {
    switch (s) {
        case ClothingStyle::GenericChic:   return "Generic Chic";
        case ClothingStyle::Leisurewear:   return "Leisurewear";
        case ClothingStyle::Urbanflash:    return "Urbanflash";
        case ClothingStyle::Businesswear:  return "Businesswear";
        case ClothingStyle::Bohemian:      return "Bohemian";
        case ClothingStyle::NomadLeathers: return "Nomad Leathers";
        default:                           return "?";
    }
}

struct ClothingPiece {
    std::string name;      // "куртка", "штаны", "верх", "обувь" и т.д.
    int         quantity = 1;
};

struct ClothingSet {
    ClothingStyle              style;
    std::vector<ClothingPiece> pieces;
};
