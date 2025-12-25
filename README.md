Dungeon Delvers is a Dwarf Fortress–inspired simulation game where you play as a sentient dungeon core, expand your underground domain, and interact with a living world of cities, factions, adventurers, and rival dungeons.

## Core Gameplay Pillars
- **Dungeon Management:** Excavate, construct, specialize rooms, and progress spawners.
- **Autonomous Simulation:** Cities, delvers, dungeons, and factions act independently.
- **Risk vs. Reward:** Expansion and aggression increase mana income but heighten threats.
- **World Persistence:** Actions permanently alter cities, populations, and rival dungeons.

## Core Loop
1. Spend **mana** to upgrade mob spawners, expand territory, unlock rooms and systems, and respond to threats.
2. Generate **mana** through combat inside owned territory, delver interactions, and successful dungeon operation.
3. Use increased mana for further expansion and specialization, escalating both power and danger.

## World Structure
- **Tile-Based Multi-Z World:** Stacked 2D tilemaps (~20 levels above ground, 1 ground level, ~40 levels below). Navigate Z-levels with Page Up / Page Down.
- **Procedural Generation:** Terrain, resources, cities, dungeons, ruins, and abandoned locations created at world start.
- **Fog of War:** Visibility tied to friendly mob line of sight; unexplored areas remain obscured.

## Time & World Simulation
- **Day / Night Cycle:** Influences city behavior, delver activity, dungeon aggression, and risk.
- **Seasons:** Affect travel speed, survival difficulty, city productivity, and delver frequency.
- **Weather Systems:** Dynamic regional weather impacts visibility, movement, combat, and hazards; severe events may damage cities or alter dungeon conditions.

## Entities & Systems
- **Dungeon Core:** Player-controlled; tracks Health (HP) and Mana. Core destruction ends the game. Growth scales with territory and spawner upgrades, unlocking new mechanics and influence.
- **Territory:** Gained in large, discrete expansions. Actions outside territory are allowed but mobs lost there do not generate mana. Territory size affects core growth, room availability, and resource placement.
- **Spawners:** Produce mobs over time with multi-tier upgrades that improve spawn rate, capacity, effectiveness, and eventually grant a rare chance to spawn advanced variants.
- **Mobs:** Autonomous inhabitants that dig, haul, construct, and fight. Behavior is role-based but player commands can override it.
- **Resource Nodes:** Placeable within territory (herbs, ores, magical resources) to support room upgrades and progression.

## Dungeon Rooms & Upgrades
Specialized rooms provide permanent bonuses such as mana boosts, new mechanics, or improved mob and spawner functionality. Examples include alchemy rooms, libraries, ritual chambers, and workshops. Room availability scales with dungeon size and progression.

## Dungeon Themes & Customization
- **Themes:** Bias favored mob types, room bonuses, mana modifiers, and visuals to encourage distinct playstyles and replayability.
- **Pre-Game Core Customization:** Choose the initial theme, starting bonuses/penalties, origin traits, and early spawner preferences to shape long-term progression.

## Cities & Population Simulation
- **Cities:** Fully simulated, tile-based settlements that expand, build housing, and react to world conditions.
- **Population AI:** Simulates individual people (not all are delvers) who live, sleep, work, and perform routines; they can die and have children. Population growth triggers housing construction.
- **Daily Schedules:** Sleep, work/activity, leisure, and delving (for delvers), with interruptions for emergencies or attacks.
- **City Morale & Stability:** Influenced by housing, safety, economy, and dungeon activity; affects delver recruitment, population growth, and unrest risk.
- **Crime, Unrest, and Decline:** Low morale can cause crime or unrest, leading to reduced productivity, population loss, or abandonment. Ruins can become dungeon spawn locations or points of interest.
- **Festivals & Events:** Temporary boosts to delver numbers/quality and morale, with potential consequences.

## Delvers
Individuals sent from cities with classes (fighter, mage, scout, etc.) and traits (brave, greedy, cautious, reckless, etc.) that shape risk tolerance, combat behavior, and response to incentives. Most do not aim to destroy the core. The player can place treasure, monsters, and encounters to influence delver behavior.

## Factions & Diplomacy
Multiple factions track approval of the dungeon based on actions such as killing delvers, providing loot, expansion behavior, or attacking other dungeons. Approval influences delver behavior, city hostility, and external threats or support.

## Foreign Dungeons
AI-controlled dungeons spawn in abandoned locations (mines, ruined houses, forts, shipwrecks) with personalities (lawful, neutral, good, evil, etc.) that affect relations and aggression. They expand, upgrade spawners, interact with factions, and can attack or be destroyed permanently.

## Combat
Simulation-driven combat influenced by terrain, positioning, equipment, weather, and time of day. Outcomes directly affect mana generation.

## UI & Player Interaction
Clicking an entity opens contextual panels showing stats, current tasks, and available actions. UI supports resolution scaling and clarity.

## Meta & Persistence
Persistent world state with save/load support and permanent consequences. Optional long-term systems include economy, trade, and inter-faction conflict.

## Technical Specifications
- **Language:** C++20 (Windows only)
- **Engine & Tooling:** SDL2 for windowing/input, OpenGL 3.3 Core Profile for rendering, CMake build system, CLion with Visual Studio toolchain, vcpkg for SDL2.
- **Visuals:** CP437 bitmap font with simple colored tiles and entities initially, evolving to full tilesets and graphics packs. External dependencies remain minimal.
