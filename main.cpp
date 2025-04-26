#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "graphic.h"

Graphics *graphics = nullptr;
Mix_Music* gMusic = nullptr;

float GetDeltaTime() {
    static Uint32 lastTime = 0;
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastTime) / 1000.0f;  // DeltaTime tính bằng giây
    lastTime = currentTime;
    return deltaTime;
}

using namespace std;

int main(int argc, char *argv[])
{
    graphics = new Graphics();
    graphics->init();
    Uint32 FrameStart;
    int FrameTime;

    gMusic = graphics->loadMusic("assets/overworld.mp3");
    if (gMusic) {
        graphics->play(gMusic, -1);
    }

    while (graphics->running())
    {


        FrameStart = SDL_GetTicks ();
        float deltaTime = GetDeltaTime();

        graphics->handleEvent();
        graphics->update();
        //graphics->update(deltaTime);

        graphics->render();

        FrameTime = SDL_GetTicks() - FrameStart;

        if (FrameDelay > FrameTime)
        {
            SDL_Delay (FrameDelay - FrameTime);
        }
    }
   if (gMusic) Mix_FreeMusic(gMusic);
    graphics->quit();
    return 0;
}
