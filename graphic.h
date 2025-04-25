#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "TextureManage.h"
#include "Map.h"
#include "ECS.h"
#include "Component.h"
#include "Vector2D.h"
    SDL_Texture* playerModel;
    SDL_Rect srcR, destR;

    Map *mariomap;

    Manager manager;

    auto& Player = manager.addEntity();

    int cameraX = 0;



class Graphics {
public:

    Graphics(){}

    ~Graphics(){}

	void logErrorAndExit(const char* msg, const char* error)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
        SDL_Quit();
    }

	void init() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            logErrorAndExit("SDL_Init", SDL_GetError());
            isRunning = false;
        }
        else {
            isRunning = true;
        }

        window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        //full screen
        //window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);

        if (window == nullptr)
            logErrorAndExit("CreateWindow", SDL_GetError());

        if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
            logErrorAndExit( "SDL_image error:", IMG_GetError());

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);

        //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));

        if (renderer == nullptr)
            logErrorAndExit("CreateRenderer", SDL_GetError());
        else
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            //man hình trắng

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);


        manager.setRenderer(renderer);

        mariomap = new Map(renderer, "assets/map/tileset_gutter.png", "assets/map/level1._Graphic Layer.csv" , "assets/map/level1._Background Color.csv", manager);

        //SDL_RenderSetScale(renderer, SCALE, SCALE);

        Player.addComponent<TransformComponent>(500, 450, 32,32,2);
        Player.addComponent<ColliderComponent>("Player");
        Player.addComponent<SpriteComponent>("assets/mario.png",renderer);
        Player.addComponent<KeyboardController>();
        Player.addComponent<PhysicsComponent>();

        Player.addGroup(groupPlayers);
        manager.addToGroup(&Player, groupPlayers);


    }
    void handleEvent () {
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            default:
                break;
        }
    }


    void render(){
        SDL_RenderClear(renderer);



        //SDL_RenderCopy(renderer, playerModel, &srcR, &destR);
        mariomap->render(renderer, cameraX);
        manager.draw();


        SDL_RenderPresent(renderer);
    }

    void update() {
        manager.refresh();
        manager.update();

        auto& Transform = Player.getComponent<TransformComponent>();

        // Tính cameraX sao cho Mario luôn nằm giữa màn hình
        cameraX = Transform.position.x + Transform.width * Transform.scale / 2 - SCREEN_WIDTH / 2;

        // Giới hạn camera không ra khỏi map
        int maxCameraX = MAP_WIDTH * TILE_SIZE * SCALE - SCREEN_WIDTH;
        if (cameraX < 0) cameraX = 0;
        if (cameraX > maxCameraX) cameraX = maxCameraX;


        Player.getComponent<KeyboardController>().update(event);
        Player.getComponent<ColliderComponent>().update();
        //Collision::checkPlayerCollisions(manager);
        Player.getComponent<TransformComponent>().update();
        //Player.getComponent<TransformComponent>().position.Add(Vector2D(1,1));




        auto* pos = &Player.getComponent<TransformComponent>();
        cout << (int)pos->position.x << " " << (int)pos->position.y << endl;
        //cout<< Player.getComponent<TransformComponent>().onGround << endl;

    }

    void update(float &deltaTime) {

    }

    void quit()
    {
        IMG_Quit();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);


        SDL_Quit();
    }

    bool running() {
        return isRunning;
    }
private:
    SDL_Event event;
    SDL_Renderer *renderer;
	SDL_Window *window;
    bool isRunning;
    int count = 0;


};


#endif // GRAPHIC_H_INCLUDED


