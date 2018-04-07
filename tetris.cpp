#include "tetris.h"

Tetris::Tetris(SDL_Renderer* renderer)
{
    spriteGrey = this->LoadTextureIMG("./res/brick_grey.png", renderer);
    spriteBlue = this->LoadTextureIMG("./res/brick_blue.png", renderer);

    L[0] = this->LoadTextureIMG("./res/level1.png", renderer);
    L[1] = this->LoadTextureIMG("./res/level2.png", renderer);
    L[2] = this->LoadTextureIMG("./res/level3.png", renderer);
    L[3] = this->LoadTextureIMG("./res/level4.png", renderer);
    L[4] = this->LoadTextureIMG("./res/level5.png", renderer);

    scoreNumber = this->LoadTextureTEXT(std::to_string(score), renderer);

    gameStart = this->LoadTextureIMG("./res/game_start.png", renderer);
    gameOver = this->LoadTextureIMG("./res/game_over.png", renderer);

    this->SetUpBorders();
    this->GeneratePiece(0);
}

void Tetris::SetUpBorders()
{
    for (unsigned int i = 0; i < MATRIX_SIZE_Y; i++)
    {
        //left border
        border_L[i].used = true;
        border_L[i].texture = spriteGrey;
        //right border
        border_R[i].used = true;
        border_R[i].texture = spriteGrey;
    }
    for (unsigned int i = 0; i < MATRIX_SIZE_X + 2; i++)
    {
        //bottom border
        border_B[i].used = true;
        border_B[i].texture = spriteGrey;
    }
}

int Tetris::RandomFigureIndex()
{
    return (rand() % 7);
}

SDL_Texture* Tetris::LoadTextureIMG(const std::string& filePath, SDL_Renderer* renderer)
{
    //Load image from file
    SDL_Surface* tmpSurface = IMG_Load(filePath.c_str());
    if (tmpSurface == nullptr)
    {
        std::cerr << "SDL2 failed to load image: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    else
    {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        if(texture == nullptr)
        {
            std::cerr << "SDL2 failed to create texture: " << SDL_GetError() << std::endl;
            return nullptr;
        }
        else
        {
            SDL_FreeSurface(tmpSurface);
            return texture;
        }
    }
}

SDL_Texture* Tetris::LoadTextureTEXT(const std::string& text, SDL_Renderer* renderer)
{
    //init TTF
    if (TTF_Init() == -1)
    {
        std::cerr << "Failed to initialize TTF: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    else
    {
        std::string fontPath = "./res/OpenSans-Bold.ttf";
        TTF_Font* font = TTF_OpenFont(fontPath.c_str(), 48);
        // check to see that the font was loaded correctly
        if (font == nullptr)
        {
            std::cerr << "Failed to load the font: " << TTF_GetError() << std::endl;
            return nullptr;
        }
        else
        {
            SDL_Color color = {0, 255, 0};
            SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
            // render the text surface
            if (surface == nullptr)
            {
                std::cerr << "Failed to render text surface: " << TTF_GetError() << std::endl;
                return nullptr;
            }
            else
            {
                // create a texture from the surface
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                if (texture == nullptr)
                {
                    std::cerr << "Unable to create texture from rendered text!" << std::endl;
                    return nullptr;
                }
                else
                {
                    textWidth = surface->w;
                    textHeight = surface->h;

                    SDL_FreeSurface(surface);
                    return texture;
                }
            }
        }
    }
}

void Tetris::GeneratePiece(unsigned int index)
{
    this->ResetPiece();

    switch (index)
    {
        case 0: // I - shape
            tetrisPiece[0][0].used = true;
            tetrisPiece[0][1].used = true;
            tetrisPiece[0][2].used = true;
            tetrisPiece[0][3].used = true;
            break;
        case 1: // L - shape
            tetrisPiece[0][0].used = true;
            tetrisPiece[0][1].used = true;
            tetrisPiece[0][2].used = true;
            tetrisPiece[1][2].used = true;
            break;
        case 2: // L_mirror - shape
            tetrisPiece[1][0].used = true;
            tetrisPiece[1][1].used = true;
            tetrisPiece[1][2].used = true;
            tetrisPiece[0][2].used = true;
            break;
        case 3: // O - shape
            tetrisPiece[0][0].used = true;
            tetrisPiece[0][1].used = true;
            tetrisPiece[1][1].used = true;
            tetrisPiece[1][0].used = true;
            break;
        case 4: // T - shape
            tetrisPiece[0][0].used = true;
            tetrisPiece[1][0].used = true;
            tetrisPiece[2][0].used = true;
            tetrisPiece[1][1].used = true;
            break;
        case 5: // Z - shape
            tetrisPiece[0][0].used = true;
            tetrisPiece[1][0].used = true;
            tetrisPiece[1][1].used = true;
            tetrisPiece[2][1].used = true;
            break;
        case 6: // Z_mirror - shape
            tetrisPiece[0][1].used = true;
            tetrisPiece[1][1].used = true;
            tetrisPiece[1][0].used = true;
            tetrisPiece[2][0].used = true;
            break;
    }
    this->CalcPieceSize();
}

void Tetris::ResetPiece()
{
    pos_X = MATRIX_SIZE_X / 2;
    pos_Y = 0;
    for (unsigned int i = 0; i < 4; i++)
    {
        for (unsigned int j = 0; j < 4; j++)
        {
            tetrisPiece[i][j].used = false;
            tetrisPiece[i][j].texture = spriteBlue;
        }
    }
}

void Tetris::CalcPieceSize()
{
    size_X = 0;
    size_H = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (tetrisPiece[i][j].used)
            {
                if (i > size_X)
                {
                    size_X = i;
                }
                if (j > size_H)
                {
                    size_H = j;
                }
            }
        }
    }
}

void Tetris::MovePiece(int dx, int dy)
{
    if (dx)
    {
        pos_X += dx;
    }
    if (dy)
    {
        pos_Y += dy;
    }
}

bool Tetris::RotationBlock()
{
    //overlapping with the right border
    if ((pos_X + size_X + 1) > MATRIX_SIZE_X)
    {
        return true;
    }
    //overlapping with the bottom border
    if ((pos_Y + size_H + 1) > MATRIX_SIZE_Y)
    {
        return true;
    }
    //look for block overlapping (in the rotated state)
    for (int xm = 0; xm < MATRIX_SIZE_X; xm++)
    {
        for (int ym = 0; ym < MATRIX_SIZE_Y; ym++)
        {
            if (tetrisMatrix[xm][ym].used)
            {

                for (int xp = 0; xp < 4; xp++)
                {
                    for (int yp = 0; yp < 4; yp++)
                    {
                        if (tetrisPiece[xp][yp].used)
                        {
                            if ((xp + pos_X) == xm && (yp + pos_Y) == ym)
                            {
                                return true;
                            }
                        }
                    }
                }

            }
        }
    }
    return false;
}

void Tetris::RotatePiece(int drot)
{
    // backup current state
    bool tmpMatrix[4][4];
    for (unsigned int i = 0; i < 4; i++)
    {
        for (unsigned int j = 0; j < 4; j++)
        {
            tmpMatrix[i][j] = tetrisPiece[i][j].used;
        }
    }
    //rotate by 90° anticlockwise
    if (drot > 0)
    {
        for (unsigned int i = 0; i < 4; i++)
        {
            for (unsigned int j = 0; j < 4; j++)
            {
                tetrisPiece[i][j].used = tmpMatrix[j][3 - i];
            }
        }
    }
    //rotate by 90° clockwise
    if (drot < 0)
    {
        for (unsigned int i = 0; i < 4; i++)
        {
            for (unsigned int j = 0; j < 4; j++)
            {
                tetrisPiece[i][j].used = tmpMatrix[3 - j][i];
            }
        }
    }
    //offset to corner 0x0 after rotating
    this->ResetToCorner();
    this->CalcPieceSize();
}

void Tetris::ResetToCorner()
{
    //find the current offset
    int offset_I = 3, offset_J = 3;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (tetrisPiece[i][j].used && i < offset_I)
            {
                offset_I = i;
            }
            if (tetrisPiece[i][j].used && j < offset_J)
            {
                offset_J = j;
            }
        }
    }
    //offset the piece to corner ixj = 0x0
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i + offset_I < 4 && j + offset_J < 4)
            {
                tetrisPiece[i][j].used = tetrisPiece[i + offset_I][j + offset_J].used;
            }
            else
            {
                tetrisPiece[i][j].used = false;
            }
        }
    }
}

void Tetris::CheckCollision()
{
    verticCollision_L = verticCollision_R = horizCollision_B = false;
    //collision with the left border
    if (pos_X == 0)
    {
        verticCollision_L = true;
    }
    //collision with the right border
    if ((pos_X + size_X + 1) == MATRIX_SIZE_X)
    {
        verticCollision_R = true;
    }
    //collision with the bottom border
    if ((pos_Y + size_H + 1) == MATRIX_SIZE_Y)
    {
        horizCollision_B = true;
    }
    //look for block collision
    for (int xm = 0; xm < MATRIX_SIZE_X; xm++)
    {
        for (int ym = 0; ym < MATRIX_SIZE_Y; ym++)
        {
            if (tetrisMatrix[xm][ym].used)
            {
                for (int xp = 0; xp < 4; xp++)
                {
                    for (int yp = 0; yp < 4; yp++)
                    {
                        if (tetrisPiece[xp][yp].used)
                        {
                            if ((xp + pos_X - 1) == xm && (yp + pos_Y) == ym)
                            {
                                verticCollision_L = true;
                            }
                            if ((xp + pos_X + 1) == xm && (yp + pos_Y) == ym)
                            {
                                verticCollision_R = true;
                            }
                            if ((xp + pos_X) == xm && (yp + pos_Y + 1) == ym)
                            {
                                horizCollision_B = true;
                            }
                        }
                    }
                }
            }
        }
    }
}

bool Tetris::CheckMatrixFull()
{
    for (int xm = 0; xm < MATRIX_SIZE_X; xm++)
    {
        if (tetrisMatrix[xm][0].used)
        {
            return true;
        }
    }
    return false;
}

int Tetris::RemoveFilledLines()
{
    int numLines = 0;
    bool filled;
    for (int ym = MATRIX_SIZE_Y; ym > -1; ym--)
    {
        //check if the line is filled
        filled = true;
        for (int xm = 0; xm < MATRIX_SIZE_X; xm++)
        {
            if (tetrisMatrix[xm][ym].used == false)
            {
                filled = false;
                break;
            }

        }
        // shift down matrix
        if (filled)
        {
            numLines += 1;
            for (int x = 0; x < MATRIX_SIZE_X; x++)
            {
                for (int y = ym; y > 0; y--)
                {
                    tetrisMatrix[x][y] = tetrisMatrix[x][y - 1];
                }
            }
            ym++;
        }
    }
    return numLines;
}

void Tetris::AddPieceToMatrix()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (tetrisPiece[i][j].used)
            {
                tetrisMatrix[i + pos_X][j + pos_Y] = tetrisPiece[i][j];
            }
        }
    }
}

void Tetris::RenderSystem(SDL_Renderer* renderer)
{
    if (level == 0)
    {
        this->RenderGameStart(renderer);
    }
    else if (level == -1)
    {
        this->RenderGameOver(renderer);
    }
    else
    {
        this->RenderTetrisPiece(renderer);

        this->RenderTetrisMatrix(renderer);

        this->RenderBorders(renderer);

        this->RenderLevelTXT(renderer);

        this->RenderScoreTXT(renderer);
    }
}

void Tetris::RenderTetrisPiece(SDL_Renderer* renderer)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (tetrisPiece[i][j].used)
            {
                SDL_Rect rect = {(MATRIX_POS_X + pos_X + i)*BLOCK_SIZE, (MATRIX_POS_Y + pos_Y + j) * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
                SDL_RenderCopy(renderer, tetrisPiece[i][j].texture, nullptr, &rect);
            }
        }
    }
}

void Tetris::RenderTetrisMatrix(SDL_Renderer* renderer)
{
    for (int i = 0; i < MATRIX_SIZE_X; i++)
    {
        for (int j = 0; j < MATRIX_SIZE_Y; j++)
        {
            if (tetrisMatrix[i][j].used)
            {
                SDL_Rect rect = {(MATRIX_POS_X + i) * BLOCK_SIZE, (MATRIX_POS_Y + j) * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
                SDL_RenderCopy(renderer, tetrisMatrix[i][j].texture, nullptr, &rect);
            }
        }
    }
}

void Tetris::RenderBorders(SDL_Renderer* renderer)
{
    //render left and right borders
    for (int i = 0; i < MATRIX_SIZE_Y; i++)
    {
        if (border_L[i].used)
        {
            SDL_Rect rect = {(MATRIX_POS_X - 1) * BLOCK_SIZE, (MATRIX_POS_Y + i) * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
            SDL_RenderCopy(renderer, border_L[i].texture, nullptr, &rect);
        }
        if (border_R[i].used)
        {
            SDL_Rect rect = {(MATRIX_POS_X + MATRIX_SIZE_X) * BLOCK_SIZE, (MATRIX_POS_Y + i) * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
            SDL_RenderCopy(renderer, border_R[i].texture, nullptr, &rect);
        }
    }
    //render bottom border
    for (int i = 0; i < MATRIX_SIZE_X + 2; i++)
    {
        if (border_B[i].used)
        {
            SDL_Rect rect = {(MATRIX_POS_X + i - 1) * BLOCK_SIZE, (MATRIX_POS_Y + MATRIX_SIZE_Y) * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
            SDL_RenderCopy(renderer, border_B[i].texture, nullptr, &rect);
        }
    }
}

void Tetris::RenderGameStart(SDL_Renderer* renderer)
{
    SDL_Rect rect = {160, 60, 500, 400};
    SDL_RenderCopy(renderer, gameStart, nullptr, &rect);
}

void Tetris::RenderGameOver(SDL_Renderer* renderer)
{
    SDL_Rect rect = {160, 60, 500, 400};
    SDL_RenderCopy(renderer, gameOver, nullptr, &rect);
}

void Tetris::RenderLevelTXT(SDL_Renderer* renderer)
{
    SDL_Rect rect = {(MATRIX_POS_X + MATRIX_SIZE_X) * BLOCK_SIZE + 100, MATRIX_POS_Y * BLOCK_SIZE + 20, 120, 40};
    SDL_RenderCopy(renderer, L[level - 1], nullptr, &rect);
}

void Tetris::RenderScoreTXT(SDL_Renderer* renderer)
{
    SDL_Rect rect = {(MATRIX_POS_X + MATRIX_SIZE_X) * BLOCK_SIZE + 140, MATRIX_POS_Y * BLOCK_SIZE + 160, textWidth, textHeight};
    SDL_RenderCopy(renderer, scoreNumber, nullptr, &rect);
}

void Tetris::CreateNewScoreTexture(SDL_Renderer* renderer)
{
    SDL_DestroyTexture(scoreNumber);
    scoreNumber = LoadTextureTEXT(std::to_string(score), renderer);
}

void Tetris::SetLevelByScore()
{
    if (level == 0 && score >= 0)
    {
        //change to Level 1
        level = 1;
    }
    if (level == 1 && score >= 20)
    {
        //change to Level 2
        level = 2;
    }
    else if (level == 2 && score >= 30)
    {
        //change to Level 3
        level = 3;
    }
    else if (level == 3 && score >= 40)
    {
        //change to Level 4
        level = 4;
    }
    else if (level == 4 && score >= 50)
    {
        //change to Level 5
        level = 5;
    }
}

Tetris::~Tetris()
{
    SDL_DestroyTexture(spriteGrey);
    SDL_DestroyTexture(spriteBlue);

    for (int i = 0; i < 5; i++)
    {
        SDL_DestroyTexture(L[i]);
    }

    SDL_DestroyTexture(scoreNumber);

    SDL_DestroyTexture(gameStart);
    SDL_DestroyTexture(gameOver);
}
