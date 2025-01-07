#include <bitset>
#include <chrono>
#include <iostream>
#include <thread>
#include "osu.hpp"
#include "process.hpp"

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
#ifndef NDEBUG
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
    std::cout << "[+] Found all memory patterns" << std::endl;

    std::cout << "[+] Starting watch loop until osu! exits" << std::endl;
    while (isProcessRunning(osuHandle)) {
        Mods mods;
        if (readOsuMenuMods(osuHandle, mods)) {
            std::cout << "Mods: " << std::bitset<32>{mods} << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "[-] osu! exited. See you next time!";

    closeProcess(osuHandle);
    return 0;
}
