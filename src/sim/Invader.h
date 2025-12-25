#pragma once

namespace dd {

    enum class InvaderKind {
        Delver
    };

    struct Invader final {
        int x = 0;
        int y = 0;
        int z = 0;

        InvaderKind kind = InvaderKind::Delver;

        int hp = 20;

        // Movement
        int move_reset = 6;
        int move_cooldown = 0;

        // Attacking (used by App.cpp)
        int atk_reset = 30;      // ticks between attacks (30 ticks @ 30 TPS = 1s)
        int atk_cooldown = 0;    // counts down to 0
        int atk_damage = 1;      // damage per attack
    };

} // namespace dd
