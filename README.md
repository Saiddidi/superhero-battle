# ⚡ Superhero Battle Simulator

A C++20 console application that simulates battles between fictional characters using a **modified ELO rating system** and **Monte Carlo simulation**.

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue?logo=c%2B%2B)
![CMake](https://img.shields.io/badge/CMake-3.16%2B-green?logo=cmake)
![License](https://img.shields.io/badge/License-MIT-yellow)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey)

---

## 📸 Preview

```
╔══════════════════════════════════════════════════════════════╗
║        ⚡   SUPERHERO  BATTLE  SIMULATOR   ⚡               ║
║              Monte Carlo Edition — C++20                     ║
╚══════════════════════════════════════════════════════════════╝

  ╔════════════════════════════════════════════════════════════╗
  ║            📊  Monte Carlo Simulation Results             ║
  ╠════════════════════════════════════════════════════════════╣
  ║  Total simulations: 10000                                 ║
  ║  Superman   ████████████████████████████  78.2%  Win      ║
  ║  Batman     ████████                      18.9%           ║
  ║  Draws:     2.9%                                          ║
  ║  🏆  Predicted Winner: Superman                           ║
  ╚════════════════════════════════════════════════════════════╝
```

---

## ✨ Features

- **Modified ELO Algorithm** — win probability computed per simulation round
- **Monte Carlo Engine** — runs up to 100,000 battle simulations
- **Critical Hit System** — based on `combat × 0.6 + speed × 0.4`
- **Luck / Environment Factor** — ±5% random variance per round
- **Weight Balancing** — prevents raw strength from dominating intelligence
- **Side-by-side stat comparison** with visual bar charts
- **JSON character database** — easily extendable
- **Manual character input** — create any character on the fly
- **Case-insensitive search** across the database
- **Colorful ANSI console UI** with progress bar

---

## 🗂️ Project Structure

```
superhero-battle/
├── main.cpp            # Entry point & interactive menu
├── Character.h         # Character struct + weightedScore()
├── BattleEngine.h      # Engine declarations
├── BattleEngine.cpp    # ELO + Monte Carlo + JSON loader + manual input
├── UI.h                # Console UI declarations
├── UI.cpp              # Progress bar, tables, color output
├── characters.json     # 10 pre-loaded characters
├── CMakeLists.txt      # Build configuration
└── README.md
```

---

## 🚀 Getting Started

### Prerequisites

| Tool | Arch Linux | Ubuntu/Debian | macOS |
|------|-----------|---------------|-------|
| GCC / G++ | `sudo pacman -S gcc` | `sudo apt install g++` | `xcode-select --install` |
| CMake | `sudo pacman -S cmake` | `sudo apt install cmake` | `brew install cmake` |
| nlohmann/json | `sudo pacman -S nlohmann-json` | `sudo apt install nlohmann-json3-dev` | `brew install nlohmann-json` |

### Build

```bash
git clone https://github.com/YOUR_USERNAME/superhero-battle.git
cd superhero-battle

mkdir build && cd build
cmake ..
cmake --build .
```

### Run

```bash
# From inside the build/ directory
./SuperheroBattle
```

> ⚠️ Make sure `characters.json` is in the same directory as the executable (CMake copies it automatically).

### Quick build (no CMake)

```bash
g++ -std=c++20 -O2 main.cpp BattleEngine.cpp UI.cpp -o SuperheroBattle
./SuperheroBattle
```

---

## ⚙️ How It Works

### Weight Balancing

| Stat | Weight | Reason |
|------|--------|--------|
| Strength | 20% | Important, but not everything |
| Speed | 18% | Reaction time decides close fights |
| Power | 17% | Special abilities & unique powers |
| Durability | 15% | Who lasts longer in battle |
| Combat | 18% | Technique can beat raw power |
| Intelligence | 12% | Planning & adapting mid-fight |

**Example:** Batman (combat=100, intelligence=100) scores **62.4** weighted points, beating a character with strength=100 but intelligence=0 who scores **58.8**.

### ELO Formula

```
P(A wins) = 1 / (1 + 10^((scoreB - scoreA) / 400))
```

### Critical Hit

```
P(critical) = (combat × 0.6 + speed × 0.4) / 200
```
A critical hit adds **+10%** to the character's score for that round.

---

## ➕ Adding Characters

Edit `characters.json` and add a new entry:

```json
{
  "id": 11,
  "name": "Vegeta",
  "publisher": "Dragon Ball",
  "alignment": "good",
  "powerstats": {
    "strength": 98,
    "speed": 98,
    "power": 98,
    "durability": 97,
    "combat": 98,
    "intelligence": 72
  }
}
```

---

## 📡 Data Sources

- [SuperHero API](https://superheroapi.com) — free API with 700+ characters
- [Kaggle Superhero Dataset](https://www.kaggle.com/search?q=superhero+dataset) — CSV format
- Marvel / DC Wikia — official lore-accurate stats

---

## 📄 License

MIT License — free to use, modify, and distribute.
