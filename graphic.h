#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
 #include <SDL_ttf.h>
#include "ECS.h"
#include "Component.h"
#include "defs.h"
#include "TextureManage.h"
#include "Map.h"
#include "Vector2D.h"
#include <cstdlib>
#include <ctime>

    SDL_Texture* playerModel;
    SDL_Rect srcR, destR;



class Graphics {
public:

    Graphics(){}
    ~Graphics(){}

	void logErrorAndExit(const char* msg, const char* error) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
        SDL_Quit();
        exit(1);
    }

	void init() {
	    //-------------Init Audio-------------
	    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
        IMG_Init(IMG_INIT_PNG);
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

        //-------------Init Window-------------
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


        if (renderer == nullptr)
            logErrorAndExit("CreateRenderer", SDL_GetError());
        else
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            //man hình trắng

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);


        manager.setRenderer(renderer);

        //-------------Init Map-------------
        mariomap = new Map(renderer, "assets/map/tileset_gutter.png", "assets/map/level1_Graphic Layer.csv" , "assets/map/level1_Background Color.csv", manager);

        //-------------Init Mario-------------
        Player = &manager.addEntity();
        Player->addComponent<TransformComponent>(50, 450, 32,32,2);
        Player->addComponent<ColliderComponent>("Player");
        Player->addComponent<PhysicsComponent>();
        Player->addComponent<SpriteComponent>("assets/mario.png",renderer);
        Player->addComponent<KeyboardController>();
        Player->addComponent<MapBoundary>();

        Player->addGroup(groupPlayers);
        manager.addToGroup(Player, groupPlayers);

        //-------------Init Text-------------
        TTF_Init();
        font = TTF_OpenFont("assets/super-mario-bros-nes.ttf", 24); // font 24px
        if (!font) {
            printf("Failed to load font: %s\n", TTF_GetError());
            isRunning = false;
        }

        //-------------Init Random Seed-------------
        srand(time(nullptr));

        startTime = SDL_GetTicks();
        lastEnemySpawnTime = SDL_GetTicks();

    }
    void handleEvent () {
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (canRestart && event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    if (mouseX >= restartButtonRect.x && mouseX < restartButtonRect.x + restartButtonRect.w &&
                        mouseY >= restartButtonRect.y && mouseY < restartButtonRect.y + restartButtonRect.h) {
                        resetGame();
                    }
                }
                break;
            default:
                break;
        }
    }


    void render(){
        SDL_RenderClear(renderer);

        mariomap->render(renderer, cameraX);
        manager.draw();

        // --- Draw UI ---
        SDL_Color textColor = {255, 255, 255, 255};

        auto renderTextTexture = [&](const std::string& text, int x, int y) {
            SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), textColor);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect destRect = {x, y, surface->w, surface->h};
            TextureManager::Draw(texture, renderer, {0, 0, surface->w, surface->h}, destRect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        };

        renderTextTexture("TIME: " + std::to_string(remainingTime), SCREEN_WIDTH - 220, 20);
        renderTextTexture("SCORE: " + std::to_string(score), 20, 20);

        //--------Hết Game--------
        if (gameOver) {
            std::string gameOverText = "GAME OVER!";
            int gameOverTextWidth;
            TTF_SizeText(font, gameOverText.c_str(), &gameOverTextWidth, nullptr);
            int gameOverX = (SCREEN_WIDTH - gameOverTextWidth) / 2;
            renderTextTexture(gameOverText, gameOverX, SCREEN_HEIGHT/2 - TTF_FontHeight(font));

            std::string finalScoreText = "FINAL SCORE: " + std::to_string(score);
            int finalScoreTextWidth;
            TTF_SizeText(font, finalScoreText.c_str(), &finalScoreTextWidth, nullptr);
            int finalScoreX = (SCREEN_WIDTH - finalScoreTextWidth) / 2;
            renderTextTexture(finalScoreText, finalScoreX, SCREEN_HEIGHT/2);

            SDL_Surface* restartSurface = TTF_RenderText_Solid(font, "RESTART", textColor);
            restartButtonTexture = SDL_CreateTextureFromSurface(renderer, restartSurface);
            restartButtonRect = {SCREEN_WIDTH/2 - restartSurface->w/2, SCREEN_HEIGHT/2 + TTF_FontHeight(font), restartSurface->w, restartSurface->h};
            TextureManager::Draw(restartButtonTexture, renderer, {0, 0, restartSurface->w, restartSurface->h}, restartButtonRect);
            SDL_FreeSurface(restartSurface);

            canRestart = true;
        } else {
            canRestart = false;
            if (restartButtonTexture != nullptr) {
                SDL_DestroyTexture(restartButtonTexture);
                restartButtonTexture = nullptr;
            }
        }

        SDL_RenderPresent(renderer);
    }

    void update() {
        Uint32 now = SDL_GetTicks();
        int elapsedSeconds = static_cast<int>((now - startTime) / 1000);
        if (!gameOver) {
            remainingTime = countdownTime - elapsedSeconds;
            if (remainingTime <= 0) {
                remainingTime = 0;
                gameOver = true;
                //std::cout << "GAME OVER! Final Score: " << score << std::endl;
            }
        }

        if (!gameOver) {
            manager.refresh();
            manager.update();

            //---------Mario----------
            if (Player != nullptr) {
                Player->getComponent<KeyboardController>().update(event);
                Player->getComponent<ColliderComponent>().update();
                Player->getComponent<PhysicsComponent>().update();
                Player->getComponent<TransformComponent>().update();
            }

            //---------Coins-----------------
            auto& coins = manager.getGroup(groupCoins);
            for (auto& coinEntity : coins) {
                auto& physics = coinEntity->getComponent<PhysicsComponent>();
                auto& Transform = coinEntity->getComponent<TransformComponent>();

                int colliderWidth = Transform.width * Transform.scale;
                int colliderHeight = Transform.height * Transform.scale;

                SDL_Rect coinRect = {
                    static_cast<int>(Transform.position.x),
                    static_cast<int>(Transform.position.y + colliderHeight),
                    colliderWidth,
                    colliderHeight
                };

                if (Player != nullptr && Player->hasComponent<ColliderComponent>()) {
                    auto& playerCollider = Player->getComponent<ColliderComponent>();
                    if (Collision::AABB(coinRect, playerCollider.collider)) {
                        coinEntity->destroy();
                        score += 100;
                    }
                }

                bool hitGround = false;
                auto& entities = manager.getGroup(groupColliders);
                for (auto& e : entities) {
                    if (!e->hasComponent<ColliderComponent>()) continue;
                    auto& other = e->getComponent<ColliderComponent>();
                    if (Collision::AABB(coinRect, other.collider)) {
                        hitGround = true;
                        break;
                    }
                }

                if (hitGround) {
                    coinEntity->destroy();
                }
            }
            //----------Spawn Coin----------
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - lastCoinSpawnTime >= coinSpawnInterval) {
                auto& currentCoins = manager.getGroup(groupCoins);
                int currentCoinCount = currentCoins.size();
                if (currentCoinCount < maxCoins) {
                    int spawnX = rand() % SCREEN_WIDTH;
                    spawnCoin(spawnX, -20, renderer, manager);
                    lastCoinSpawnTime = currentTime;
                }
            }
            //----------Spawn Enemy----------
            if (score >= 1000 || remainingTime < 100) {
                Uint32 now = SDL_GetTicks();
                if (now - lastEnemySpawnTime >= enemySpawnInterval) {
                    auto& currentEnemies = manager.getGroup(groupEnemies);
                    if (currentEnemies.size() < maxEnemies) { // Kiểm tra số lượng enemies hiện tại
                        spawnEnemy(renderer, manager);
                        lastEnemySpawnTime = now;
                    }
                }
            }
            //----------Move Enemies----------
            auto& enemies = manager.getGroup(groupEnemies);
            for (auto& enemyEntity : enemies) {
                if (enemyEntity->hasComponent<TransformComponent>()) {
                    auto& Transform = enemyEntity->getComponent<TransformComponent>();
                    if (Transform.position.x < -32) {
                        Transform.velocity.x = 1.0f; // Đặt vận tốc dương để di chuyển vào màn hình
                    } else if (Transform.position.x > SCREEN_WIDTH + 32) {
                        Transform.velocity.x = -1.0f; // Đặt vận tốc âm để di chuyển vào màn hình
                    }
                    enemyEntity->getComponent<TransformComponent>().update(); // Áp dụng vận tốc (đã được điều chỉnh nếu cần)
                }
            }

            //----------Enemy - Player Collision----------
            if (Player != nullptr && Player->hasComponent<ColliderComponent>()) {
                auto& playerCollider = Player->getComponent<ColliderComponent>();
                for (auto& enemyEntity : enemies) {
                    if (enemyEntity->hasComponent<ColliderComponent>()) {
                        auto& enemyCollider = enemyEntity->getComponent<ColliderComponent>();
                        if (Collision::AABB(playerCollider.collider, enemyCollider.collider)) {
                            score -= 200;
                            if (score < 0) {
                                score = 0;
                            }
                            // Tùy chọn: Thêm xử lý khác khi va chạm
                            enemyEntity->destroy();
                        }
                    }
                }
            }
        }

        auto* pos = &Player->getComponent<TransformComponent>();
        //cout << (int)pos->position.x << " " << (int)pos->position.y  << endl;
        //cout<< Player.getComponent<TransformComponent>().onGround << endl;
    }
    void spawnCoin(int x, int y, SDL_Renderer* renderer, Manager& manager) {

        auto& coin = manager.addEntity();

        coin.addComponent<TransformComponent>(x, y, 16, 16, SCALE);
        coin.addComponent<SpriteComponent>("assets/img/other_imgs/coin6.png", renderer);
        coin.addComponent<ColliderComponent>("Coin");
        coin.addComponent<PhysicsComponent>();
        coin.addGroup(groupCoins);
    }

    void spawnEnemy(SDL_Renderer* renderer, Manager& manager) {
        auto& enemy = manager.addEntity();
        int side = rand() % 2;
        float x = (side == 0) ? -32.0f : static_cast<float>(SCREEN_WIDTH + 32);
        float y = 450.0f;
        enemy.addComponent<TransformComponent>(x, y, 28, 28, 2);
        enemy.addComponent<SpriteComponent>("assets/img/npc_imgs/creature1.png", renderer);
        enemy.addComponent<ColliderComponent>("Enemy");
        enemy.addComponent<PhysicsComponent>(); // Thêm PhysicsComponent (cho trọng lực và va chạm Y)
        float moveSpeed = 1.0f + static_cast<float>(rand()) / RAND_MAX * 0.5f;
        enemy.addComponent<EnemyAIComponent>(moveSpeed); // Thêm EnemyAIComponent (cho di chuyển ngang)
        if (side == 1) {
            enemy.getComponent<EnemyAIComponent>().direction = -1.0f;
        }
        enemy.addGroup(groupEnemies);
    }

    void quit()
    {
        delete mariomap;
        mariomap = nullptr;

        IMG_Quit();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        Mix_HaltMusic();
        Mix_CloseAudio();
        Mix_Quit();

        TTF_CloseFont(font);
        TTF_Quit();

        SDL_Quit();
    }

    void resetGame() {
        gameOver = false;
        isRunning = true;
        remainingTime = countdownTime;
        score = 0;
        startTime = SDL_GetTicks();
        lastEnemySpawnTime = SDL_GetTicks();

        manager.clearAllEntities();
        manager.refresh();

        // Khởi tạo lại Player ở vị trí ban đầu
        Player = &manager.addEntity();
        Player->addComponent<TransformComponent>(50, 450, 32,32,2);
        Player->addComponent<ColliderComponent>("Player");
        Player->addComponent<SpriteComponent>("assets/mario.png",renderer);
        Player->addComponent<PhysicsComponent>();
        Player->addComponent<KeyboardController>();
        Player->addComponent<MapBoundary>();

        Player->addGroup(groupPlayers);
        manager.addToGroup(Player, groupPlayers);

        // Khởi tạo lại bản đồ (nếu cần)
        delete mariomap;
        mariomap = new Map(renderer, "assets/map/tileset_gutter.png", "assets/map/level1_Graphic Layer.csv" , "assets/map/level1_Background Color.csv", manager);

        // Đặt lại thời gian spawn coin
        lastCoinSpawnTime = SDL_GetTicks();

        // Đảm bảo nút restart không còn hiển thị và không còn tương tác được
        canRestart = false;
        if (restartButtonTexture != nullptr) {
            SDL_DestroyTexture(restartButtonTexture);
            restartButtonTexture = nullptr;
        }
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
    Map *mariomap;

    Manager manager;

    Entity* Player = nullptr;

    int cameraX = 0;
    SDL_Event event;
    SDL_Renderer *renderer = nullptr;
	SDL_Window *window = nullptr;
    bool isRunning;
    int frameCount = 0;

    TTF_Font* font = nullptr;
    int score = 0;
    int countdownTime = 5; // seconds
    int remainingTime = 5; // Tgian chạy game
    bool gameOver = false;

    Uint32 startTime = 0;
    Uint32 lastCoinSpawnTime = 0;      // Thời điểm spawn coin gần nhất
    Uint32 coinSpawnInterval = 500;// Khoảng cách spawn ban đầu (ms)

    int currentCoinCount = 0;   // Số lượng coin spawn mỗi lần
    int maxCoins = 5;

    bool canRestart = false; // ResetGame
    SDL_Rect restartButtonRect; // Nút reset
    SDL_Texture* restartButtonTexture = nullptr;
    SDL_Surface* restartButtonSurface = nullptr;

    // Enemy spawning variables
    Uint32 lastEnemySpawnTime;
    Uint32 enemySpawnInterval = 1500; // Adjust the interval as needed (milliseconds)
    int maxEnemies = 3; // Optional: Limit the number of enemies
};

#endif // GRAPHIC_H_INCLUDED

