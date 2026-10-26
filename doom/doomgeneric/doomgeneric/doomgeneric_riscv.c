#include "doomgeneric.h"
#include "doomkeys.h"
#include "address.h"

static unsigned char scancode_to_doom(uint8_t code){
    if (code >= 4  && code <= 29) return 'a' + (code - 4);    // A..Z -> строчные
    if (code >= 30 && code <= 38) return '1' + (code - 30);   // 1..9
    if (code >= 58 && code <= 67) return KEY_F1 + (code - 58); // F1..F10

    switch (code) {
        case 39:  return '0';
        case 40:  return KEY_ENTER;
        case 41:  return KEY_ESCAPE;
        case 42:  return KEY_BACKSPACE;
        case 43:  return KEY_TAB;
        case 44:  return KEY_USE;          // пробел — открыть дверь
        case 45:  return KEY_MINUS;
        case 46:  return KEY_EQUALS;
        case 54:  return KEY_STRAFE_L;     // ,
        case 55:  return KEY_STRAFE_R;     // .
        case 68:  return KEY_F11;
        case 69:  return KEY_F12;
        case 79:  return KEY_RIGHTARROW;
        case 80:  return KEY_LEFTARROW;
        case 81:  return KEY_DOWNARROW;
        case 82:  return KEY_UPARROW;
        case 224: case 228: return KEY_FIRE;    // Ctrl
        case 225: case 229: return KEY_RSHIFT;  // Shift — бег
        case 226: case 230: return KEY_RALT;    // Alt — стрейф стрелками
    }
    return 0;
}

void DG_Init(void) {
    return;
}

void DG_DrawFrame(void) {
    for (int i = 0; i < DOOMGENERIC_RESX * DOOMGENERIC_RESY; i++) {
        SCREEN[i] = DG_ScreenBuffer[i];
    }
}

void DG_SleepMs(uint32_t ms) {
    uint32_t start = *TIMER;
    while (*TIMER - start < ms) {}
}

uint32_t DG_GetTicksMs(void) {
    return *TIMER;
}

int DG_GetKey(int* pressed, unsigned char* doomKey) {
    for (;;) {
        uint32_t event = *KEYBOARD;
        if (event == 0) return 0;

        unsigned char key = scancode_to_doom(event & 0xFF);
        if (key == 0) continue;

        *pressed = (event >> 8) & 1;
        *doomKey = key;
        return 1;
    }
}

void DG_SetWindowTitle(const char* title) {
    return;
}

int main(int argc, char** argv) {
    char* args[] = { "doom", "-iwad", "doom.wad", 0 };

    doomgeneric_Create(3, args);

    for(;;){
        doomgeneric_Tick();
    }
}