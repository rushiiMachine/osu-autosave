#pragma once

#include <cstddef>
#include <vector>
#include "process.hpp"

bool readProcessRegion(void* procHandle, const MemoryRegion& region, std::vector<std::byte>& buffer);

bool readProcessU32(void* procHandle, void* baseAddress, uint32_t* out);
