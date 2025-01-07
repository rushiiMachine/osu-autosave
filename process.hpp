#pragma once

#include <vector>
#include <cstdint>

struct MemoryRegion {
    void* baseAddress;
    std::size_t size;
};

uint32_t findOsuPid();

void* openProcess(uint32_t pid);

void closeProcess(void* procHandle);

bool isProcessRunning(void* procHandle);

std::vector<MemoryRegion> getProcessRegions(void* procHandle);

bool readProcessMemory(void* procHandle, void* baseAddress, size_t length, void* output);
