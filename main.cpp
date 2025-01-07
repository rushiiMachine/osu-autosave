#include <bitset>
#include <chrono>
#include <iostream>
#include <thread>
#include "memory.hpp"
#include "pattern.hpp"
#include "process.hpp"
#include "scan.hpp"

struct Patterns {
    Pattern<"C8 FF ? ? ? ? ? 81 0D ? ? ? ? ? 08 00 00", 0x9> menuModsPtr;
} PATTERNS;

struct Pointers {
    uint32_t* menuMods;
} POINTER_CACHE;

bool initializeCache(void* osuHandle, const std::vector<MemoryRegion>& regions) {
    std::cout << "[-] Searching osu! memory for patterns" << std::endl;
    ScanResult scan;

    if (!PATTERNS.menuModsPtr.search(osuHandle, regions, scan)) {
        std::cerr << "[!] Failed to find menuModsPtr" << std::endl;
        return false;
    }

    POINTER_CACHE.menuMods = *reinterpret_cast<uint32_t**>(scan.data.data() +
                                                           scan.foundIdx +
                                                           PATTERNS.menuModsPtr.offset);

    return true;
}

int main() {
    auto osuPid = findOsuPid();
    auto* osuHandle = openProcess(osuPid);
    if (!osuHandle) {
        std::cerr << "[!] Failed to open osu! process! Is osu! not running or running with admin privileges?"
                  << std::endl;
        return 1;
    }
    std::cout << "[+] osu! pid: " << osuPid << std::endl;

    const auto regions = getProcessRegions(osuHandle);
#ifdef DEBUG
    for (const auto& region: regions) {
        std::cout << "[-] Found memory region at 0x"
                  << std::uppercase << std::hex << region.baseAddress
                  << " with size 0x" << std::uppercase << std::hex << region.size
                  << std::endl;
    }
#endif

    if (!initializeCache(osuHandle, regions)) {
        std::cerr << "[!] Failed to locate memory patterns!" << std::endl;
        closeProcess(osuHandle);
        return 1;
    }

    std::cout << "Found menuModsPtr at" << PATTERNS.menuModsPtr.foundPointer;

    std::cout << "[+] Starting watch loop until osu! exits" << std::endl;
    while (isProcessRunning(osuHandle)) {
        uint32_t mods = 0;
        if (!readProcessU32(osuHandle, POINTER_CACHE.menuMods, &mods)) {
            std::cout << "[!] Failed to read mods value" << std::endl;
            exit(1);
        }
        std::cout << "Mods: " << std::bitset<32>{mods} << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "[-] osu! exited. See you next time!";

    closeProcess(osuHandle);
    return 0;
}
