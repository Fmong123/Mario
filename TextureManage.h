#ifndef TEXTUREMANAGE_H_INCLUDED
#define TEXTUREMANAGE_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include <SDL_log.h>

class TextureManager {

public:
    static SDL_Texture* Load(const char* filePath, SDL_Renderer* renderer)
    {
        SDL_Surface* tempSurface = IMG_Load(filePath);
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
        return tex;
    }

    static void Draw(SDL_Texture* texture, SDL_Renderer* renderer, SDL_Rect srcRect, SDL_Rect destRect, double angle, SDL_Point* center, SDL_RendererFlip flip)
    {
        SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, angle, center, flip);
    }

    // Hàm Draw hiện tại có thể gọi hàm này với các giá trị mặc định
    static void Draw(SDL_Texture* texture, SDL_Renderer* renderer, SDL_Rect srcRect, SDL_Rect destRect)
    {
        Draw(texture, renderer, srcRect, destRect, 0.0, nullptr, SDL_FLIP_NONE);
    }
};
#endif // TEXTUREMANAGE_H_INCLUDED
