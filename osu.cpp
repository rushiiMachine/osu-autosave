#include <iostream>
#include "memory.hpp"
#include "osu.hpp"

bool initializeCache(void* osuHandle, const std::vector<MemoryRegion>& regions) {
    std::cout << "[-] Searching osu! memory for patterns" << std::endl;
    ScanResult scan;

#define FILL_PATTERN(patternName) \
    if (!PATTERNS.patternName.search(osuHandle, regions, scan)) { \
        std::cerr << "[!] Failed to find " #patternName << std::endl; \
        return false; \
    }

    FILL_PATTERN(menuModsPtr);

    POINTER_CACHE.menuMods = *reinterpret_cast<uint32_t**>(scan.data.data() +
                                                           scan.foundIdx +
                                                           PATTERNS.menuModsPtr.offset);

    return true;
}

bool readOsuMenuMods(void* osuHandle, Mods& mods) {
    if (!readProcessU32(osuHandle, POINTER_CACHE.menuMods, reinterpret_cast<uint32_t*>(&mods))) {
        std::cerr << "[!] Failed to read menuMods" << std::endl;
        return false;
    }
    return true;
}
