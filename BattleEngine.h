#pragma once
#include "Character.h"
#include <vector>
#include <string>
#include <optional>

// ============================================================
//  BattleEngine.h — Core battle simulation engine
// ============================================================

struct BattleResult {
    std::string winnerName;
    int   winsA        = 0;
    int   winsB        = 0;
    int   draws        = 0;
    int   totalRounds  = 0;
    float winRateA     = 0.0f;
    float winRateB     = 0.0f;
    float drawRate     = 0.0f;
    float avgScoreA    = 0.0f;
    float avgScoreB    = 0.0f;
};

class BattleEngine {
public:
    // ── Data management ─────────────────────────────────────
    bool loadFromJSON(const std::string& filepath);
    void addCharacter(const Character& c);
    std::optional<Character> findByName(const std::string& name) const;
    void listAllCharacters() const;

    // ── Simulation engine ────────────────────────────────────
    BattleResult runMonteCarlo(const Character& a,
                               const Character& b,
                               int simulations = 10000) const;

    // ── Manual input ─────────────────────────────────────────
    Character inputManually() const;

private:
    std::vector<Character> db_;   // Local character database

    // Modified ELO formula for single-round win probability
    float eloWinProbability(float scoreA, float scoreB) const;

    // Critical hit based on combat + speed stats
    bool criticalHit(const Character& c, float rand01) const;

    // Luck / environment factor ± 5%
    float luckFactor() const;
};
