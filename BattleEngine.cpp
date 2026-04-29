#include "BattleEngine.h"
#include "UI.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <cmath>
#include <cctype>
#include <limits>

using json = nlohmann::json;

// ============================================================
//  Thread-local random number generator
// ============================================================
static thread_local std::mt19937 rng{ std::random_device{}() };

static float randFloat(float lo = 0.0f, float hi = 1.0f) {
    std::uniform_real_distribution<float> dist(lo, hi);
    return dist(rng);
}

// ============================================================
//  Load characters from JSON file
// ============================================================
bool BattleEngine::loadFromJSON(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Cannot open file: " << filepath << "\n";
        return false;
    }

    json j;
    try { file >> j; }
    catch (const json::parse_error& e) {
        std::cerr << "[ERROR] JSON parse error: " << e.what() << "\n";
        return false;
    }

    for (const auto& item : j) {
        Character c;
        c.id        = item.value("id", 0);
        c.name      = item.value("name", "Unknown");
        c.publisher = item.value("publisher", "Unknown");
        c.alignment = item.value("alignment", "neutral");

        auto& s = item["powerstats"];
        c.stats.strength     = s.value("strength",     50.0f);
        c.stats.speed        = s.value("speed",        50.0f);
        c.stats.power        = s.value("power",        50.0f);
        c.stats.durability   = s.value("durability",   50.0f);
        c.stats.combat       = s.value("combat",       50.0f);
        c.stats.intelligence = s.value("intelligence", 50.0f);

        db_.push_back(c);
    }

    std::cout << "[OK] Loaded " << db_.size()
              << " characters from " << filepath << "\n";
    return true;
}

// ============================================================
//  Add a character to the database
// ============================================================
void BattleEngine::addCharacter(const Character& c) {
    db_.push_back(c);
}

// ============================================================
//  Case-insensitive name search
// ============================================================
std::optional<Character> BattleEngine::findByName(const std::string& name) const {
    std::string lower = name;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    for (const auto& c : db_) {
        std::string cLower = c.name;
        std::transform(cLower.begin(), cLower.end(), cLower.begin(), ::tolower);
        if (cLower == lower) return c;
    }
    return std::nullopt;
}

// ============================================================
//  List all characters in the database
// ============================================================
void BattleEngine::listAllCharacters() const {
    std::cout << "\n══════════════════════════════════════════════\n";
    std::cout << "       Characters in Database\n";
    std::cout << "══════════════════════════════════════════════\n";
    for (const auto& c : db_) {
        std::cout << "  [" << c.id << "] " << c.name
                  << "  (" << c.publisher << ")\n";
    }
    std::cout << "══════════════════════════════════════════════\n\n";
}

// ============================================================
//  Modified ELO win probability
//  Formula: P(A) = 1 / (1 + 10^((scoreB - scoreA) / K))
//  K = 400 — scaling factor (higher = more upsets possible)
// ============================================================
float BattleEngine::eloWinProbability(float scoreA, float scoreB) const {
    constexpr float K = 400.0f;
    return 1.0f / (1.0f + std::pow(10.0f, (scoreB - scoreA) / K));
}

// ============================================================
//  Luck / environment factor  ± 5%
// ============================================================
float BattleEngine::luckFactor() const {
    return randFloat(-0.05f, 0.05f);
}

// ============================================================
//  Critical hit probability
//  P(crit) = (combat × 0.6 + speed × 0.4) / 200
//  Max: combat=100, speed=100 → 50% chance
// ============================================================
bool BattleEngine::criticalHit(const Character& c, float rand01) const {
    float prob = (c.stats.combat * 0.6f + c.stats.speed * 0.4f) / 200.0f;
    return rand01 < prob;
}

// ============================================================
//  Monte Carlo simulation — main engine
// ============================================================
BattleResult BattleEngine::runMonteCarlo(const Character& a,
                                          const Character& b,
                                          int simulations) const {
    BattleResult result;
    result.totalRounds = simulations;

    float baseScoreA = a.weightedScore();
    float baseScoreB = b.weightedScore();
    float sumA = 0, sumB = 0;

    UI::showProgressBar(simulations);

    for (int i = 0; i < simulations; ++i) {
        // Apply luck factor to base scores
        float sA = baseScoreA + baseScoreA * luckFactor();
        float sB = baseScoreB + baseScoreB * luckFactor();

        // Critical hits add +10% to score
        if (criticalHit(a, randFloat())) sA *= 1.10f;
        if (criticalHit(b, randFloat())) sB *= 1.10f;

        sumA += sA;
        sumB += sB;

        // ELO win probability for this round
        float pA   = eloWinProbability(sA, sB);
        float roll = randFloat();

        if (std::abs(pA - 0.5f) < 0.02f) {
            result.draws++;           // Near 50/50 → draw
        } else if (roll < pA) {
            result.winsA++;
        } else {
            result.winsB++;
        }

        UI::updateProgressBar(i + 1, simulations);
    }

    std::cout << "\n";

    result.winRateA  = (float)result.winsA  / simulations * 100.0f;
    result.winRateB  = (float)result.winsB  / simulations * 100.0f;
    result.drawRate  = (float)result.draws  / simulations * 100.0f;
    result.avgScoreA = sumA / simulations;
    result.avgScoreB = sumB / simulations;
    result.winnerName = (result.winsA >= result.winsB) ? a.name : b.name;

    return result;
}

// ============================================================
//  Manual character input
// ============================================================
Character BattleEngine::inputManually() const {
    Character c;
    static int autoId = 1000;
    c.id = autoId++;

    std::cout << "\n  > Character name: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, c.name);

    std::cout << "  > Publisher / Source: ";
    std::getline(std::cin, c.publisher);

    auto getVal = [](const std::string& label) -> float {
        float v = -1;
        while (v < 0 || v > 100) {
            std::cout << "    " << label << " (0-100): ";
            std::cin >> v;
            if (v < 0 || v > 100)
                std::cout << "    [!] Please enter a value between 0 and 100\n";
        }
        return v;
    };

    std::cout << "\n  Enter character stats:\n";
    c.stats.strength     = getVal("Strength    ");
    c.stats.speed        = getVal("Speed       ");
    c.stats.power        = getVal("Power       ");
    c.stats.durability   = getVal("Durability  ");
    c.stats.combat       = getVal("Combat      ");
    c.stats.intelligence = getVal("Intelligence");

    return c;
}
