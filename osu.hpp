#pragma once

#include <cinttypes>
#include "pattern.hpp"

// Offsets from https://github.com/tosuapp/tosu/blob/89db801a5afde0ea11cf92f0698c1ec0dcebe7b8/packages/tosu/src/memory/stable.ts
static struct Patterns {
    Pattern<"C8 FF ? ? ? ? ? 81 0D ? ? ? ? ? 08 00 00", 0x9> menuModsPtr;
} PATTERNS;

static struct Pointers {
    uint32_t* menuMods;
} POINTER_CACHE;

enum Mods : uint32_t {
    NoMod = 0,
    NoFail = 1 << 0,
    Easy = 1 << 1,
    TouchDevice = 1 << 2,
    Hidden = 1 << 3,
    HardRock = 1 << 4,
    SuddenDeath = 1 << 5,
    DoubleTime = 1 << 6,
    Relax = 1 << 7,
    HalfTime = 1 << 8,
    Nightcore = 1 << 9,
    Flashlight = 1 << 10,
    Autoplay = 1 << 11,
    SpunOut = 1 << 12,
    Relax2 = 1 << 13,
    Perfect = 1 << 14,
    Key4 = 1 << 15,
    Key5 = 1 << 16,
    Key6 = 1 << 17,
    Key7 = 1 << 18,
    Key8 = 1 << 19,
    FadeIn = 1 << 20,
    Random = 1 << 21,
    Cinema = 1 << 22,
    Target = 1 << 23,
    Key9 = 1 << 24,
    Key10 = 1 << 25,
    Key1 = 1 << 26,
    Key3 = 1 << 27,
    Key2 = 1 << 28,
    LastMod = 1 << 29,
    keyMod = Key1 | Key2 | Key3 | Key4 | Key5 | Key6 | Key7 | Key8 | Key9 | Key10,
    KeyModUnranked = Key1 | Key2 | Key3 | Key9 | Key10,
    FreeModAllowed =
    NoFail | Easy | Hidden | HardRock | SuddenDeath | Flashlight | FadeIn | Relax | Relax2 | SpunOut | keyMod,
    ScoreIncreaseMods = Hidden | HardRock | DoubleTime | Flashlight | FadeIn
};

bool initializeCache(void* osuHandle, const std::vector<MemoryRegion>& regions);

bool readOsuMenuMods(void* osuHandle, Mods& mods);
