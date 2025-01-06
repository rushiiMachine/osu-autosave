#include <cstddef>
#include <vector>
#include "memory.hpp"
#include "process.hpp"

bool readProcessRegion(void* procHandle, const MemoryRegion& region, std::vector<std::byte>& buffer) {
    buffer.clear();
    buffer.resize(region.size);

    return readProcessMemory(procHandle, region.baseAddress, region.size, buffer.data());
}

bool readProcessU32(void* procHandle, void* baseAddress, uint32_t* out) {
    return readProcessMemory(procHandle, baseAddress, 4, out);
}
