#pragma once
#include <string>

// ============================================================
//  Character.h — Superhero data model
// ============================================================

struct Stats {
    float strength     = 0;   // Physical strength
    float speed        = 0;   // Movement & reaction speed
    float power        = 0;   // Special abilities / powers
    float durability   = 0;   // Ability to withstand damage
    float combat       = 0;   // Fighting skill & technique
    float intelligence = 0;   // Tactical thinking & planning
};

struct Character {
    int         id;
    std::string name;
    std::string publisher;
    std::string alignment;   // "good" / "bad" / "neutral"
    Stats       stats;

    // Weighted score calculation.
    // Weights are designed to prevent raw strength from
    // dominating over intelligence and combat skill.
    float weightedScore() const {
        // Weights sum to 1.0
        constexpr float W_STRENGTH     = 0.20f;
        constexpr float W_SPEED        = 0.18f;
        constexpr float W_POWER        = 0.17f;
        constexpr float W_DURABILITY   = 0.15f;
        constexpr float W_COMBAT       = 0.18f;  // Skill matters as much as strength
        constexpr float W_INTELLIGENCE = 0.12f;  // Tactics can overcome raw power

        return stats.strength     * W_STRENGTH
             + stats.speed        * W_SPEED
             + stats.power        * W_POWER
             + stats.durability   * W_DURABILITY
             + stats.combat       * W_COMBAT
             + stats.intelligence * W_INTELLIGENCE;
    }
};
