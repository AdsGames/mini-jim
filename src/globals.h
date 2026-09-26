#pragma once

// All defined presets put here
// tile Attributes
inline constexpr int solid = 1;
inline constexpr int harmful = 2;
inline constexpr int light = 3;
inline constexpr int slide = 4;
inline constexpr int shadow = 5;
inline constexpr int attribute_max = 6;

// Fixed physics step, matches the asw desktop timestep (8ms)
inline constexpr float FIXED_STEP_MS = 8.0F;

// Longest frame time simulated at once
inline constexpr float MAX_LAG_MS = 100.0F;

// Absorbs float error when asw passes exactly one step
inline constexpr float STEP_EPSILON_MS = 0.01F;

extern bool single_player;
extern int levelOn;
inline constexpr int levelCount = 6;
