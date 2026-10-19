#include "keyboard.h"

void Keyboard::push(bool pressed, uint8_t key){
    uint32_t event = key | (pressed ? 0x100 : 0);
    queue.push_back(event);
    if (queue.size() > max_queue){
        queue.pop_front();
    }
}

uint32_t Keyboard::read32(uint32_t addr){
    if (queue.size() > 0){
        uint32_t event = queue.front();
        queue.pop_front();
        return event;
    } else {
        return 0;
    }
}