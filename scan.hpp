#pragma once

#include <span>
#include <vector>
#include <ctfp/ctfp.h>
#include "memory.hpp"

struct ScanResult {
    MemoryRegion memoryRegion;
    std::vector<std::byte> data;
    size_t foundIdx;
    void* foundRealPtr;
};

template<ctfp::string Pattern>
ScanResult scanMemory(void* procHandle, const std::vector<MemoryRegion>& regions) {
    constexpr static auto pattern = ctfp::build<Pattern>();
    auto buffer = std::vector<std::byte>();

    for (const auto& region: regions) {
        if (!readProcessRegion(procHandle, region, buffer))
            continue;

        auto memory = std::span{buffer};
        auto result = std::search(memory.begin(), memory.end(), pattern.begin(), pattern.end());

        if (result == memory.end())
            continue;

        auto index = std::distance(memory.begin(), result);
        return ScanResult{
                region,
                std::move(buffer),
                static_cast<size_t>(index),
                static_cast<void*>(static_cast<uint8_t*>(region.baseAddress) + index)
        };
    }

    return ScanResult{};
}
