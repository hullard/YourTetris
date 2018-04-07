#ifndef TETRIS_H
#define TETRIS_H

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

struct Block
{
    SDL_Texture* texture;
    bool used = false;
};

class Tetris
{
public:
    Tetris(SDL_Renderer* renderer);
    int RandomFigureIndex();
    void GeneratePiece(unsigned int index);
    void MovePiece(int dx, int dy);
    void RotatePiece(int drot);
    void CheckCollision();
    bool CheckMatrixFull();
    bool RotationBlock();
    inline bool Get_verticCollision_L() {return verticCollision_L; }
    inline bool Get_verticCollision_R() {return verticCollision_R; }
    inline bool Get_horizCollision_B() {return horizCollision_B; }
    inline int& Get_level() {return level; };
    inline int& Get_score() {return score; };
    void AddPieceToMatrix();
    void SetLevelByScore();
    void CreateNewScoreTexture(SDL_Renderer* renderer);
    void RenderSystem(SDL_Renderer* renderer);
    int RemoveFilledLines();
    ~Tetris();
private:
    static const int MATRIX_POS_X = 2, MATRIX_POS_Y = 1;
    static const int MATRIX_SIZE_X = 10, MATRIX_SIZE_Y = 20;
    static const int BLOCK_SIZE = 20;

    Block tetrisPiece[4][4];
    Block tetrisMatrix[MATRIX_SIZE_X][MATRIX_SIZE_Y];
    Block border_L[MATRIX_SIZE_Y];
    Block border_R[MATRIX_SIZE_Y];
    Block border_B[MATRIX_SIZE_X + 2];
    //Left X and Y pos are set fixed within piece matrix, size is calculated after each rotation
    int pos_X, pos_Y;
    int size_X, size_H;
    bool verticCollision_L, verticCollision_R, horizCollision_B;
    //score + level
    int score = 0, level = 0;
    int textWidth, textHeight;
    //define grey sprite or borders, blue sprite for tetris pieces
    SDL_Texture* spriteGrey;
    SDL_Texture* spriteBlue;
    SDL_Texture* L[5];
    SDL_Texture* scoreNumber;
    SDL_Texture* gameStart;
    SDL_Texture* gameOver;
    //loading textures from files and texts
    SDL_Texture* LoadTextureIMG(const std::string& filePath, SDL_Renderer* renderer);
    SDL_Texture* LoadTextureTEXT(const std::string& text, SDL_Renderer* renderer);

    void CalcPieceSize();
    void ResetToCorner();
    void SetUpBorders();
    void ResetPiece();

    void RenderTetrisPiece(SDL_Renderer* renderer);
    void RenderTetrisMatrix(SDL_Renderer* renderer);
    void RenderBorders(SDL_Renderer* renderer);
    void RenderGameStart(SDL_Renderer* renderer);
    void RenderGameOver(SDL_Renderer* renderer);
    void RenderLevelTXT(SDL_Renderer* renderer);
    void RenderScoreTXT(SDL_Renderer* renderer);
};

#endif // TETRIS_H
