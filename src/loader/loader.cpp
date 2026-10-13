#include "loader.h"
#include <fstream>
#include <cstdio>

bool loadBinary(Bus& bus, const char* path, uint32_t addr){
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        printf("Cannot open %s\n", path);
        return false;
    }

    char byte;
    while (file.get(byte)) {
        bus.write8(addr++, uint8_t(byte));
    }
    return true;
}