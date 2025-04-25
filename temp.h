#ifndef TEMP_H_INCLUDED
#define TEMP_H_INCLUDED

    void prepareScene(SDL_Texture * background)
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy( renderer, background, NULL, NULL);
    }

    void presentScene()
    {
        SDL_RenderClear(renderer);
    }

    SDL_Texture *loadTexture(const char *filename)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

        SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
        if (texture == NULL)
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());

        return texture;
    }

    void renderTexture(SDL_Texture *texture, int x, int y)
    {
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

        SDL_RenderCopy(renderer, texture, NULL, &dest);
    }

    void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y)
    {
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        dest.w = src->w;
        dest.h = src->h;

        SDL_RenderCopy(renderer, texture, src, &dest);
    }



    Bạn đã nói:
    void handleEvent () {
        SDL_Event event;
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_RIGHT)
                    cameraX += 8;
                else if (event.key.keysym.sym == SDLK_LEFT)
                    cameraX -= 8;

                // Giới hạn cameraX không vượt ra ngoài map
                if (cameraX < 0) cameraX = 0;
                if (cameraX > 240 * 16 - SCREEN_WIDTH)
                    cameraX = 240 * 16 - SCREEN_WIDTH;
                break;
            default:
                break;
        }
    }


    destRect = {x * TILE_SIZE - cameraX, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};


    inline ComponentID getUniqueComponentID() {
        static ComponentID lastID = 0;
        return lastID++;
    }

    template<typename T>
    inline ComponentID getComponentID() {
        static ComponentID typeID = getUniqueComponentID();
        return typeID;
    }

    void setPos (int x, int y)
    {
        xpos = x;
        ypos = y;
    }

    void x(int x) {xpos = x;}
    void y (int y) {ypos = y;}
    int x() { return position.x; }
    int y() { return position.y; }
#endif // TEMP_H_INCLUDED
