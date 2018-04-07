#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

// handle display

class Display
{
public:
    static unsigned int REFRESH_COUNT;

    Display(int width, int height, const std::string& title);
    void Clear(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
    inline SDL_Renderer* Get_renderer() {return renderer; }
    void Update();
    virtual ~Display();
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
};

#endif // DISPLAY_H
