#pragma once

#include <cassert>
#include <cstddef>
#include <ctfp/ctfp.h>
#include "memory.hpp"
#include "scan.hpp"

template<
        /* The pattern to search for in process memory. */
        ctfp::string SearchPattern,
        /* An offset to apply after finding the start of a search pattern. */
        size_t Offset = 0x0
>
struct Pattern {
    /* The result pointer into process memory that was found. This includes the offset. */
    void* foundPointer;

    /* The offset in bytes that was applied to the pointer. */
    size_t offset;

    Pattern() : foundPointer(nullptr), offset(Offset) {}

    bool search(void* procHandle, const std::vector<MemoryRegion>& regions, ScanResult& scanResult) {
        ScanResult result = scanMemory<SearchPattern>(procHandle, regions);
        this->foundPointer = static_cast<uint8_t*>(result.foundRealPtr) + Offset;
        scanResult = std::move(result);
        return result.isOk();
    }
};
