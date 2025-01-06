#if defined(WIN32) || defined(_WIN32)

#include <iostream>
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>
#include "process.hpp"

uint32_t findOsuPid() {
    bool found = false;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    PROCESSENTRY32 processEntry{
            .dwSize = sizeof(PROCESSENTRY32),
    };

    if (Process32First(snapshot, &processEntry)) {
        do {
            if (strcmp(processEntry.szExeFile, "osu!.exe") == 0) {
                found = true;
                break;
            }
        } while (Process32Next(snapshot, &processEntry));
    }

    CloseHandle(snapshot);
    return found ? processEntry.th32ProcessID : 0;
}

void* openProcess(uint32_t pid) {
    if (!pid) return nullptr;
    return OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid);
}

void closeProcess(void* handle) {
    CloseHandle(handle);
}

bool isProcessRunning(void* handle) {
    DWORD returnCode{};
    if (!GetExitCodeProcess(handle, &returnCode))
        return false;

    return returnCode == STILL_ACTIVE;
}

std::vector<MemoryRegion> getProcessRegions(void* handle) {
    std::vector<MemoryRegion> regions;
    MEMORY_BASIC_INFORMATION info;

    for (uint8_t* address = nullptr;
         VirtualQueryEx(handle, address, &info, sizeof(info)) != 0;
         address += info.RegionSize) {

        if ((info.State & MEM_COMMIT) == 0 || (info.Protect & (PAGE_READWRITE | PAGE_EXECUTE_READWRITE)) == 0) {
            continue;
        }

        regions.push_back(MemoryRegion{info.BaseAddress, info.RegionSize});
    }

    return regions;
}

bool readProcessMemory(void* procHandle, void* baseAddress, size_t length, void* output) {
    SIZE_T read = 0;
    if (!ReadProcessMemory(procHandle, baseAddress, output, length, &read)) {
        std::cout << "Failed to read memory at "
                  << "0x" << std::uppercase << std::hex << baseAddress
                  << " due to error: 0x" << GetLastError()
                  << ", read 0x" << read << " bytes!"
                  << std::endl;
    }
    return read == length;
}

#endif
