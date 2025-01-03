#if defined(WIN32) || defined(_WIN32)

#include <vector>
#include <Windows.h>
#include <TlHelp32.h>
#include "process.h"

uint32_t findOsuPid() {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    PROCESSENTRY32 processEntry{
            .dwSize = sizeof(PROCESSENTRY32),
            .th32ProcessID = 0,
    };

    if (Process32First(snapshot, &processEntry)) {
        do {
            if (strcmp(processEntry.szExeFile, "osu!.exe") == 0)
                break;
        } while (Process32Next(snapshot, &processEntry));
    }

    CloseHandle(snapshot);
    return processEntry.th32ProcessID;
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

        regions.push_back(MemoryRegion{reinterpret_cast<void*>(info.BaseAddress), info.RegionSize});
    }

    return regions;
}

bool readProcessRegion(void* handle, const MemoryRegion& region, void* buffer) {
    return ReadProcessMemory(handle, region.baseAddress, buffer, region.size, 0) == 0;
}

#endif
