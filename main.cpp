#include <chrono>
#include <thread>
#include <span>
#include <vector>
#include <iostream>
#include <ctfp/ctfp.h>
#include "process.h"

template<ctfp::string Pattern>
void* scanMemory(void* handle, const std::vector<MemoryRegion>& regions) {
    constexpr static auto pattern = ctfp::build<Pattern>();
    auto buffer = std::vector<std::byte>();

    for (const auto& region: regions) {
        buffer.clear();
        buffer.reserve(region.size);
        if (!readProcessRegion(handle, region, reinterpret_cast<void*>(buffer.data())))
            continue;

        auto memory = std::span{buffer};
        auto result = std::search(memory.begin(), memory.end(), pattern.begin(), pattern.end());

        if (result == memory.end())
            continue;

        return reinterpret_cast<void*>(std::addressof(*result));
    }

    return nullptr;
}

int main() {
    uint32_t osuPid = findOsuPid();
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

        void* menuMods = scanMemory<"C8FF?????810D?????080000">(osuHandle, regions);
        std::cout << "Found menuMods at 0x"
                  << std::uppercase << std::hex << menuMods
                  << std::endl;
        if (menuMods) {
            std::cout << "Menu mods: " << (**((uint64_t**) menuMods)) << std::endl;
        }
    }

    while (osuHandle && isProcessRunning(osuHandle)) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "osu! running!" << std::endl;
    }

    closeProcess(osuHandle);
    return 0;
}
