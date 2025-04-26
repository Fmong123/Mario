#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "ECS.h"
#include "Component.h"
#include "defs.h"
#include "TextureManage.h"
#include "Map.h"
#include "Vector2D.h"
    SDL_Texture* playerModel;
    SDL_Rect srcR, destR;

    Map *mariomap;

    Manager manager;

    auto& Player = manager.addEntity();



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
	    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
        IMG_Init(IMG_INIT_PNG);
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

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

        mariomap = new Map(renderer, "assets/map/tileset_gutter.png", "assets/map/level1_Graphic Layer.csv" , "assets/map/level1_Background Color.csv", manager);

        //SDL_RenderSetScale(renderer, SCALE, SCALE);


        Player.addComponent<TransformComponent>(50, 450, 32,32,2);
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

        Player.getComponent<KeyboardController>().update(event, cameraX);

        Player.getComponent<ColliderComponent>().update();
        Player.getComponent<TransformComponent>().update();

        updateCamera();


        auto* pos = &Player.getComponent<TransformComponent>();
        cout << (int)pos->position.x << " " << (int)pos->position.y << " " << cameraX << endl;
        //cout<< Player.getComponent<TransformComponent>().onGround << endl;

        //int maxCameraX = (MAP_WIDTH * TILE_SIZE * SCALE) - SCREEN_WIDTH;


        //cameraX = std::clamp(cameraX, 0, maxCameraX);

    }

    void update(float &deltaTime) {

    }

    int getCameraX() const {
        return cameraX;
    }

    void setCameraX(int x) {
        int maxCameraX = MAP_WIDTH * TILE_SIZE * SCALE - SCREEN_WIDTH;

        if (x < 0) cameraX = 0;
        else if (x > maxCameraX) cameraX = maxCameraX;
        else cameraX = x;
    }

    void quit()
    {
        IMG_Quit();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        Mix_HaltMusic();
        Mix_CloseAudio();
        Mix_Quit();
        SDL_Quit();
    }

    bool running() {
        return isRunning;
    }

    Mix_Music* loadMusic(const std::string& path) {
        Mix_Music* music = Mix_LoadMUS(path.c_str());
        if (!music) {
            std::cerr << "Failed to load BGM: " << Mix_GetError() << std::endl;
        }
        return music;
    }
    void play(Mix_Music* music, int loops) {
        if (Mix_PlayMusic(music, loops) == -1) {
            std::cerr << "Failed to play BGM: " << Mix_GetError() << std::endl;
        }
    }
private:
    int cameraX = 0;
    SDL_Event event;
    SDL_Renderer *renderer;
	SDL_Window *window;
    bool isRunning;
    int count = 0;

   void updateCamera() {
        auto& T = Player.getComponent<TransformComponent>();
        float marioX = T.position.x;  // Vị trí của Mario
        float marioVelX = T.velocity.x;  // Tốc độ di chuyển của Mario

        // Đảm bảo Mario luôn ở giữa màn hình khi chưa đến giữa
        if (marioX < SCREEN_WIDTH / 2) {
            cameraX = 0;  // Nếu Mario chưa tới giữa, camera giữ vị trí ban đầu
        } else {
            // Khi Mario đến giữa màn hình, camera sẽ bắt đầu cuộn theo tốc độ của Mario
            // Giữ Mario ở giữa
            cameraX = static_cast<int>(marioX - SCREEN_WIDTH / 2);

            // Giới hạn camera để không vượt quá bản đồ
            int maxCameraX = MAP_WIDTH * TILE_SIZE * SCALE - SCREEN_WIDTH;

            // Giới hạn camera để không vượt qua bản đồ
            if (cameraX < 0) {
                cameraX = 0;
            } else if (cameraX > maxCameraX) {
                cameraX = maxCameraX;
            }
        }

        // Đảm bảo rằng camera di chuyển theo tốc độ của Mario nếu Mario đã đến giữa màn hình
        if (marioX >= SCREEN_WIDTH / 2) {
            cameraX += static_cast<int>(marioVelX);  // Camera di chuyển theo tốc độ của Mario
        }
    }
};

#endif // GRAPHIC_H_INCLUDED


