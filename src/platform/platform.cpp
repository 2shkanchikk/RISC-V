#include "platform.h"
#include "../devices/keyboard.h"
#include <cstdio>
#include <SDL2/SDL.h>
#include <iostream>

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
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return;
    }
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window Error: " << SDL_GetError() << "\n";
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        std::cerr << "Renderer Error: " << SDL_GetError() << "\n";
        return;
    }
    SDL_RenderSetLogicalSize(renderer, logicWidth, logicHeight);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, logicWidth, logicHeight);
    if (!texture) {
        std::cerr << "Texture Error: " << SDL_GetError() << "\n";
        return;
    }
}

Platform::~Platform(){
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Platform::update(const uint32_t* video){
    uint32_t pixels[64*32]{};
    for (int i = 0; i < 2048; ++i){
        pixels[i] = (video[i] == 1) ? 0xFFFFFFFF : 0x000000FF;
    }

    SDL_UpdateTexture(texture, nullptr, pixels, 320 * 4);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

bool Platform::processInput(Keyboard& kb){
    SDL_Event event{};
    while (SDL_PollEvent(&event)){
        if (event.type == SDL_QUIT) return false;

        if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && event.key.repeat == 0) {
            SDL_Scancode code = event.key.keysym.scancode;
            if (code > 0 && code < 256) {
                kb.push(event.type == SDL_KEYDOWN, uint8_t(code));
            }
        }
    }

    return true;
}