#pragma once

#include <vector>
#include <cstdint>

struct MemoryRegion {
    void* baseAddress;
    std::size_t size;
};

uint32_t findOsuPid();

void* openProcess(uint32_t pid);

void closeProcess(void* handle);

bool isProcessRunning(void* handle);

std::vector<MemoryRegion> getProcessRegions(void* handle);

bool readProcessRegion(void* handle, const MemoryRegion& region, void* buffer);
