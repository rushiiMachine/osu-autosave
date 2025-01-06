#include <bitset>
#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>
#include "memory.hpp"
#include "process.hpp"
#include "scan.hpp"

int main() {
    uint32_t osuPid = findOsuPid();
    if (!osuPid) return 1;
    void* osuHandle = openProcess(osuPid);
    std::cout << "osu! pid: " << osuPid << std::endl;

    if (osuHandle) {
        const auto regions = getProcessRegions(osuHandle);
        for (const auto& region: regions) {
            std::cout << "Found memory region at 0x"
                      << std::uppercase << std::hex << region.baseAddress
                      << " with size 0x" << std::uppercase << std::hex << region.size
                      << std::endl;
        }

        ScanResult menuModsPtrScan = scanMemory<"C8FF?????810D?????080000">(osuHandle, regions);
        if (menuModsPtrScan.foundRealPtr) {
            auto* menuModsPtr = *reinterpret_cast<uint32_t**>(
                    menuModsPtrScan.data.data() + menuModsPtrScan.foundIdx + 0x9);

            uint32_t menuMods;
            assert(readProcessU32(osuHandle, menuModsPtr, &menuMods) && "Failed to read mods value");

            std::cout << "Found menuModsPtr at 0x"
                      << std::uppercase << std::hex << menuModsPtrScan.foundRealPtr
                      << ", mods: " << std::bitset<32>{menuMods}
                      << std::endl;
        }
    }

    while (osuHandle && isProcessRunning(osuHandle)) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "osu! running!" << std::endl;
    }

    closeProcess(osuHandle);
    return 0;
}
