#include "BattleEngine.h"
#include "UI.h"
#include <iostream>
#include <string>
#include <limits>

// ============================================================
//  main.cpp — Entry point & interactive menu
// ============================================================

// Helper: select a character from DB or manual input
Character pickCharacter(BattleEngine& engine, const std::string& label) {
    std::cout << UI::Color::BOLD
              << "\n  ── Select " << label << " ──\n"
              << UI::Color::RESET;
    std::cout << "  [1] Search database\n";
    std::cout << "  [2] Enter manually\n";
    std::cout << "  Choice: ";

    int choice = 0;
    std::cin >> choice;

    if (choice == 2) {
        return engine.inputManually();
    }

    // Database search loop
    while (true) {
        std::cout << "  Character name: ";
        std::string name;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, name);

        auto result = engine.findByName(name);
        if (result.has_value()) {
            UI::showCharacterCard(result.value());
            return result.value();
        }
        std::cout << UI::Color::RED
                  << "  [!] \"" << name << "\" not found — try again.\n"
                  << UI::Color::RESET;
    }
}

int main() {
    BattleEngine engine;
    UI::showBanner();

    // Load character database
    std::cout << UI::Color::CYAN
              << "  Loading character database...\n"
              << UI::Color::RESET;

    if (!engine.loadFromJSON("characters.json")) {
        std::cout << UI::Color::YELLOW
                  << "  [!] characters.json not found — manual input only.\n"
                  << UI::Color::RESET;
    }

    int choice = -1;
    while (choice != 0) {
        UI::showMainMenu();
        std::cin >> choice;

        switch (choice) {

        // ── Battle (DB search) ─────────────────────────────
        case 1: {
            Character charA = pickCharacter(engine, "Fighter 1");
            Character charB = pickCharacter(engine, "Fighter 2");

            std::cout << "\n  Simulations? (default 10000, max 100000): ";
            int sims = 0;
            std::cin >> sims;
            if (sims <= 0 || sims > 100000) sims = 10000;

            UI::showSideBySide(charA, charB);

            BattleResult result = engine.runMonteCarlo(charA, charB, sims);
            UI::showBattleResult(charA, charB, result);

            std::cout << "  Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            break;
        }

        // ── Battle (manual input) ──────────────────────────
        case 2: {
            std::cout << UI::Color::YELLOW
                      << "\n  ── Enter Fighter 1 ──\n"
                      << UI::Color::RESET;
            Character charA = engine.inputManually();

            std::cout << UI::Color::YELLOW
                      << "\n  ── Enter Fighter 2 ──\n"
                      << UI::Color::RESET;
            Character charB = engine.inputManually();

            std::cout << "\n  Simulations? (default 10000, max 100000): ";
            int sims = 0;
            std::cin >> sims;
            if (sims <= 0 || sims > 100000) sims = 10000;

            UI::showSideBySide(charA, charB);

            BattleResult result = engine.runMonteCarlo(charA, charB, sims);
            UI::showBattleResult(charA, charB, result);

            std::cout << "  Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            break;
        }

        // ── List all characters ────────────────────────────
        case 3:
            engine.listAllCharacters();
            break;

        // ── Weight explanation ─────────────────────────────
        case 4:
            UI::showWeightExplanation();
            break;

        case 0:
            std::cout << UI::Color::CYAN
                      << "\n  Thanks for using Superhero Battle Simulator! 🏆\n\n"
                      << UI::Color::RESET;
            break;

        default:
            std::cout << UI::Color::RED
                      << "  [!] Invalid choice.\n"
                      << UI::Color::RESET;
        }
    }

    return 0;
}
