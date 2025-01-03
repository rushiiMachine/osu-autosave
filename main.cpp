#include <span>
#include <iostream>
#include <stdint.h>
#include <ctfp/ctfp.h>

int main() {
    uint8_t bytes[] = {0xFF, 0xDD, 0xAA, 0x35, 0xFF, 0x2F};
    std::span<std::byte> bytesSpan = {reinterpret_cast<std::byte*>(&bytes), (sizeof(bytes) / sizeof(uint8_t))};

    auto match = ctfp::find<"FF?AA">(bytesSpan);
    std::cout << "0x" << std::uppercase << std::hex << match << std::endl;
    std::cout << std::uppercase << std::hex << int(*reinterpret_cast<uint8_t*>(match)) << std::endl;
}
