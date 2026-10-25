#pragma once
#include <cstdint>
#include <string>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
class Keyboard;

class Platform{
    SDL_Window* window{nullptr};
    SDL_Renderer* renderer{nullptr};
    SDL_Texture* texture{nullptr};
    int logicWidth{};

    public:
        Platform(const char* title, int windowWidth, int windowHeight, int logicWidth, int logicHeight);
        ~Platform();

        void update(const uint32_t* video);
        bool processInput(Keyboard& kb);
};

void save_ppm(const char* filename, const uint32_t* pixels, uint32_t ScreenWidth, uint32_t ScreenHeight);