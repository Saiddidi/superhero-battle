#include "UI.h"
#include "BattleEngine.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace UI::Color;

// ============================================================
//  Helper: visual stat bar  e.g. [████████░░░░]
// ============================================================
static std::string statBar(float val, int width = 20) {
    int filled = static_cast<int>((val / 100.0f) * width);
    std::string bar = "[";
    for (int i = 0; i < width; ++i)
        bar += (i < filled) ? "\xE2\x96\x88" : "\xE2\x96\x91";  // █ / ░
    bar += "]";
    return bar;
}

// ============================================================
//  Banner
// ============================================================
void UI::showBanner() {
    std::cout << BOLD << CYAN;
    std::cout << R"(
╔══════════════════════════════════════════════════════════════╗
║                                                              ║
║        ⚡   SUPERHERO  BATTLE  SIMULATOR   ⚡               ║
║              Monte Carlo Edition — C++20                     ║
║                                                              ║
╚══════════════════════════════════════════════════════════════╝
)" << RESET << "\n";
}

// ============================================================
//  Main menu
// ============================================================
void UI::showMainMenu() {
    std::cout << BOLD << WHITE;
    std::cout << "  ┌──────────────────────────────────────┐\n";
    std::cout << "  │             Main  Menu               │\n";
    std::cout << "  ├──────────────────────────────────────┤\n";
    std::cout << "  │  [1]  Search from database           │\n";
    std::cout << "  │  [2]  Enter characters manually      │\n";
    std::cout << "  │  [3]  List all characters            │\n";
    std::cout << "  │  [4]  How weight balancing works     │\n";
    std::cout << "  │  [0]  Exit                           │\n";
    std::cout << "  └──────────────────────────────────────┘\n" << RESET;
    std::cout << "\n  Your choice: ";
}

// ============================================================
//  Progress bar
// ============================================================
void UI::showProgressBar(int total) {
    std::cout << "\n  " << YELLOW << BOLD
              << "Running " << total << " simulations...\n" << RESET;
    std::cout << "  [";
    for (int i = 0; i < 50; ++i) std::cout << "\xE2\x96\x91";
    std::cout << "] 0%\r  [";
    std::cout.flush();
}

void UI::updateProgressBar(int current, int total) {
    int percent = (current * 100) / total;
    int filled  = (current * 50)  / total;

    std::cout << "\r  " << GREEN << "[";
    for (int i = 0; i < 50; ++i)
        std::cout << (i < filled ? "\xE2\x96\x88" : "\xE2\x96\x91");
    std::cout << "] " << BOLD << percent << "%" << RESET;
    std::cout.flush();
}

// ============================================================
//  Character card
// ============================================================
void UI::showCharacterCard(const Character& c) {
    std::cout << BOLD << MAGENTA
              << "\n  ╔══════════════════════════════════════╗\n"
              << "  ║  " << std::left << std::setw(36) << c.name   << "║\n"
              << "  ║  " << std::setw(36) << c.publisher            << "║\n"
              << "  ╠══════════════════════════════════════╣\n" << RESET;

    auto row = [](const std::string& label, float val) {
        std::cout << "  ║  " << CYAN << std::left << std::setw(14) << label << RESET
                  << " " << YELLOW << statBar(val, 14) << RESET
                  << " " << BOLD << std::right << std::setw(5) << val << RESET << " ║\n";
    };

    row("Strength   ", c.stats.strength);
    row("Speed      ", c.stats.speed);
    row("Power      ", c.stats.power);
    row("Durability ", c.stats.durability);
    row("Combat     ", c.stats.combat);
    row("Intelligence", c.stats.intelligence);

    std::cout << "  ╠══════════════════════════════════════╣\n"
              << "  ║  " << GREEN << "Weighted Score: "
              << BOLD << std::fixed << std::setprecision(2)
              << c.weightedScore()
              << std::string(16, ' ') << RESET << "║\n"
              << "  ╚══════════════════════════════════════╝\n\n";
}

// ============================================================
//  Side-by-side stat comparison
// ============================================================
void UI::showSideBySide(const Character& a, const Character& b) {
    auto bar = [](float val, int w = 12) -> std::string {
        int f = static_cast<int>((val / 100.0f) * w);
        std::string s;
        for (int i = 0; i < w; ++i)
            s += (i < f ? "\xE2\x96\x88" : "\xE2\x96\x91");
        return s;
    };

    std::cout << BOLD << "\n";
    std::cout << "  ╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "  ║              Side-by-Side Stat Comparison                 ║\n";
    std::cout << "  ╠══════════════════╦════════════╦════════╦══════════════════╣\n";

    std::cout << "  ║ " << CYAN    << std::left  << std::setw(16) << a.name << RESET << " ║"
              << "   Stat     " << " ║"
              << "  VS  " << "  ║"
              << MAGENTA << std::right << std::setw(16) << b.name << RESET << " ║\n";

    std::cout << "  ╠══════════════════╬════════════╬════════╬══════════════════╣\n";

    struct Row { std::string label; float vA; float vB; };
    Row rows[] = {
        {"Strength    ", a.stats.strength,     b.stats.strength},
        {"Speed       ", a.stats.speed,        b.stats.speed},
        {"Power       ", a.stats.power,        b.stats.power},
        {"Durability  ", a.stats.durability,   b.stats.durability},
        {"Combat      ", a.stats.combat,       b.stats.combat},
        {"Intelligence", a.stats.intelligence, b.stats.intelligence},
    };

    for (auto& r : rows) {
        std::string colA = (r.vA > r.vB) ? GREEN : (r.vA < r.vB ? RED : WHITE);
        std::string colB = (r.vB > r.vA) ? GREEN : (r.vB < r.vA ? RED : WHITE);

        std::cout << "  ║ " << colA << std::right << std::setw(3) << r.vA
                  << " " << bar(r.vA) << RESET << " ║"
                  << " " << YELLOW << std::left << std::setw(10) << r.label << RESET << " ║"
                  << " " << bar(r.vB) << " " << colB << std::setw(3) << r.vB << RESET << " ║\n";
    }

    std::cout << "  ╠══════════════════╬════════════╬════════╬══════════════════╣\n";
    std::cout << "  ║ " << GREEN << std::right << std::setw(6)
              << std::fixed << std::setprecision(1) << a.weightedScore()
              << " (weighted)" << RESET << " ║"
              << "  Total    " << " ║"
              << " (weighted) " << MAGENTA << std::setw(6)
              << b.weightedScore() << RESET << " ║\n";
    std::cout << "  ╚══════════════════╩════════════╩════════╩══════════════════╝\n\n"
              << RESET;
}

// ============================================================
//  Battle result report
// ============================================================
void UI::showBattleResult(const Character& a, const Character& b,
                           const BattleResult& result) {
    std::cout << BOLD << "\n";
    std::cout << "  ╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "  ║            📊  Monte Carlo Simulation Results             ║\n";
    std::cout << "  ╠════════════════════════════════════════════════════════════╣\n";
    std::cout << "  ║  Total simulations: " << std::setw(6) << result.totalRounds
              << "                                   ║\n";
    std::cout << "  ╠════════════════════════════════════════════════════════════╣\n";

    // Fighter A results
    std::string barA(static_cast<int>(result.winRateA / 2), '\xE2');
    int barLenA = static_cast<int>(result.winRateA / 2);
    std::cout << "  ║  " << CYAN << std::left << std::setw(16) << a.name << RESET << "  ";
    std::cout << GREEN;
    for (int i = 0; i < barLenA; ++i) std::cout << "\xE2\x96\x88";
    std::cout << RESET << "\n";
    std::cout << "  ║    Wins: " << GREEN << std::setw(5) << result.winsA << RESET
              << "  Rate: " << GREEN << BOLD << std::setw(6) << std::fixed
              << std::setprecision(1) << result.winRateA << "%" << RESET
              << "  Avg Score: " << std::setw(6) << result.avgScoreA << "          ║\n";

    std::cout << "  ╠════════════════════════════════════════════════════════════╣\n";

    // Fighter B results
    int barLenB = static_cast<int>(result.winRateB / 2);
    std::cout << "  ║  " << MAGENTA << std::left << std::setw(16) << b.name << RESET << "  ";
    std::cout << RED;
    for (int i = 0; i < barLenB; ++i) std::cout << "\xE2\x96\x88";
    std::cout << RESET << "\n";
    std::cout << "  ║    Wins: " << RED << std::setw(5) << result.winsB << RESET
              << "  Rate: " << RED << BOLD << std::setw(6) << result.winRateB << "%" << RESET
              << "  Avg Score: " << std::setw(6) << result.avgScoreB << "          ║\n";

    std::cout << "  ╠════════════════════════════════════════════════════════════╣\n";
    std::cout << "  ║  Draws: " << YELLOW << std::setw(5) << result.draws << RESET
              << "  Rate: " << YELLOW << std::setw(5) << result.drawRate << "%" << RESET
              << "                                ║\n";

    std::cout << "  ╠════════════════════════════════════════════════════════════╣\n";
    std::cout << "  ║  🏆  Predicted Winner: " << BOLD << YELLOW
              << std::setw(20) << result.winnerName << RESET << "              ║\n";
    std::cout << "  ╚════════════════════════════════════════════════════════════╝\n\n"
              << RESET;
}

// ============================================================
//  Weight balancing explanation
// ============================================================
void UI::showWeightExplanation() {
    std::cout << BOLD << CYAN
              << "\n  ═══ Weight Balancing System ═══\n\n" << RESET;

    std::cout << WHITE <<
R"(  Problem:
  If raw strength had a large weight (e.g. 40%), characters like
  Superman would always defeat Batman, ignoring Batman's superior
  combat skill and intelligence.

  Solution — distributed weights:
)" << RESET;

    struct W { const char* name; float pct; const char* reason; };
    W weights[] = {
        {"Strength    ", 20.0f, "Important, but not everything"},
        {"Speed       ", 18.0f, "Reaction time decides close fights"},
        {"Power       ", 17.0f, "Special abilities & unique powers"},
        {"Durability  ", 15.0f, "Who lasts longer in battle"},
        {"Combat      ", 18.0f, "Technique can beat raw power"},
        {"Intelligence", 12.0f, "Planning & adapting mid-fight"},
    };

    for (auto& w : weights) {
        std::cout << "  " << YELLOW << std::left << std::setw(16) << w.name
                  << RESET << BOLD << std::setw(5) << w.pct << "%" << RESET
                  << "  →  " << w.reason << "\n";
    }

    std::cout << "\n  " << GREEN
              << "Result: Batman (combat=100, intelligence=100) can defeat\n"
              << "  a physically stronger character if they are slower and less tactical.\n"
              << RESET << "\n";
}
