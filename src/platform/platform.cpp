#include "platform.h"
#include <cstdio>
#include <SDL2/SDL.h>

void save_ppm(const char* filename, const uint32_t* pixels, uint32_t ScreenWidth, uint32_t ScreenHeight){
    FILE* f = fopen(filename, "wb");
    fprintf(f, "P6\n%u %u\n255\n", ScreenWidth, ScreenHeight);
    if (!f) {
        printf("Cannot create %s\n", filename);
        return;
    }

    for (uint32_t i = 0; i < ScreenWidth * ScreenHeight; i++) {
        uint32_t p = pixels[i];
        fputc((p >> 16) & 0xFF, f);
        fputc((p >> 8)  & 0xFF, f);
        fputc(p & 0xFF, f);
    }

    fclose(f);
}

Platform::Platform(const char* title, int windowWidth, int windowHeight, int logicWidth, int logicHeight){

}

Platform::~Platform(){
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}