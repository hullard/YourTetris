// YourTetris (a very simple Tetris application by using classes) - written by David Hullar

// Including headers
#include <iostream>
#include <chrono>
#include <SDL2/SDL.h>

// Including classes
#include "display.h"
#include "tetris.h"

// Global constants by preprocessor
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// game state + key events
bool start, quit;
int dx, dy, drot;

// timed move + register keyboard events
void TimedMove(unsigned int level);
void RegisterKeys();
void HandleActions(Tetris* tetris);

// Timing and clock
auto tStart = std::chrono::high_resolution_clock::now();

int main (int argc, char* args[])
{
    Display display(SCREEN_WIDTH, SCREEN_HEIGHT, "YourTetris");

    SDL_Renderer* renderer = display.Get_renderer();

    Tetris tetris(renderer);

    while (!quit)
    {
        RegisterKeys();

        if (start)
        {
            tetris.SetLevelByScore();
            int level = tetris.Get_level();

            TimedMove(level);

            HandleActions(&tetris);

            if (dx || dy || drot)
            {
                tetris.CheckCollision();

                if (tetris.Get_horizCollision_B())
                {
                    tetris.AddPieceToMatrix();

                    int numLines = tetris.RemoveFilledLines();

                    if (numLines)
                    {
                        tetris.Get_score() += numLines;
                        tetris.CreateNewScoreTexture(renderer);
                    }

                    if (tetris.CheckMatrixFull())
                    {
                        start = false;
                        tetris.Get_level() = -1;//game over
                    }
                    else
                    {
                        int newFigureIndex = tetris.RandomFigureIndex();
                        tetris.GeneratePiece(newFigureIndex);
                    }
                }
            }
        }

        display.Clear(0, 0, 0, 255);

        tetris.RenderSystem(renderer);

        display.Update();

        Display::REFRESH_COUNT += 1;

        SDL_Delay(25);
    }
    return 0;
}


void RegisterKeys()
{
    dx = dy = drot = 0;
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_LEFT:
                    dx = -1;
                    break;
                case SDLK_RIGHT:
                    dx = 1;
                    break;
                case SDLK_DOWN:
                    dy = 1;
                    break;
                case SDLK_UP:
                    drot = 1;
                    break;
                case SDLK_RETURN:
                    start = true;
                    break;
            }
        }
        if (event.type == SDL_QUIT)
        {
            quit = true;
        }
    }
}

void TimedMove(unsigned int level)
{
    auto tFinish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = tFinish - tStart;

    if (elapsed.count() > double(1.0 / level))
    {
        dy = 1;
        //std::cout << "FPS: " << int(Display::REFRESH_COUNT / elapsed.count()) << std::endl;
        tStart = std::chrono::high_resolution_clock::now();
        Display::REFRESH_COUNT = 0;
    }
}

void HandleActions(Tetris* tetris)
{
    if (dx < 0 && tetris->Get_verticCollision_L() == false)
    {
        tetris->MovePiece(dx, 0);
    }

    if (dx > 0 && tetris->Get_verticCollision_R() == false)
    {
        tetris->MovePiece(dx, 0);
    }

    if (dy > 0 && tetris->Get_horizCollision_B() == false)
    {
        tetris->MovePiece(0, dy);
    }

    if (drot)
    {
        tetris->RotatePiece(drot);

        if (tetris->RotationBlock() == true)
        {
            tetris->RotatePiece(-drot);
        }
    }
}
