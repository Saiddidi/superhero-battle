#pragma once
#include "Character.h"
#include <string>

// ============================================================
//  UI.h — Interactive console interface declarations
// ============================================================

struct BattleResult;  // forward declaration

namespace UI {

    void showBanner();
    void showMainMenu();
    void showProgressBar(int total);
    void updateProgressBar(int current, int total);
    void showCharacterCard(const Character& c);
    void showSideBySide(const Character& a, const Character& b);
    void showBattleResult(const Character& a, const Character& b,
                          const BattleResult& result);
    void showWeightExplanation();

    // ANSI color codes
    namespace Color {
        inline constexpr const char* RESET   = "\033[0m";
        inline constexpr const char* RED     = "\033[31m";
        inline constexpr const char* GREEN   = "\033[32m";
        inline constexpr const char* YELLOW  = "\033[33m";
        inline constexpr const char* BLUE    = "\033[34m";
        inline constexpr const char* CYAN    = "\033[36m";
        inline constexpr const char* WHITE   = "\033[37m";
        inline constexpr const char* BOLD    = "\033[1m";
        inline constexpr const char* MAGENTA = "\033[35m";
    }

}  // namespace UI
