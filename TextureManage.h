#ifndef TEXTUREMANAGE_H_INCLUDED
#define TEXTUREMANAGE_H_INCLUDED

#include "graphic.h"
#include "defs.h"

class TextureManager {

public:
    static SDL_Texture* Load(const char* filePath, SDL_Renderer* renderer)
    {
        SDL_Surface* tempSurface = IMG_Load(filePath);
        // load anh
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
        // tao texture
        SDL_FreeSurface(tempSurface);

        return tex;

    }
    static void Draw(SDL_Texture* texture, SDL_Renderer* renderer, SDL_Rect srcRect, SDL_Rect destRect)
    {
        SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
    }
};
#endif // TEXTUREMANAGE_H_INCLUDED
