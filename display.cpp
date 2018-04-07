#include "display.h"

unsigned int Display::REFRESH_COUNT = 0;

Display::Display(int width, int height, const std::string& title)
{
    // Initialize SDL2
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
        std::cerr << "SDL2 failed to initialize: " << SDL_GetError() << std::endl;
    }
    // Create window
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (window == nullptr)
    {
        std::cerr << "SDL2 failed to create window: " << SDL_GetError() << std::endl;
    }
    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        std::cerr << "SDL2 failed to create renderer: " << SDL_GetError() << std::endl;
    }
    SDL_RenderSetLogicalSize(renderer, width, height);
}

void Display::Clear(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
    // clear the renderer with the color
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);
}

void Display::Update()
{
    // Copy render to the screen
    SDL_RenderPresent(renderer);
}

Display::~Display()
{
    // Destroy everything
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
