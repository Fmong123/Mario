#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include "defs.h"
#include "TextureManage.h"
#include <bits/stdc++.h>
#include "graphic.h"
#include "ECS.h"
#include "Component.h"

using namespace std;

class Map
{
public:
    Map(SDL_Renderer* renderer, const char* tilesetPath, const char* graphicLayerPath, const char* backgroundLayerPath, Manager &mManager)
        : manager(mManager)
    {

        tileset = TextureManager::Load(tilesetPath, renderer);
        graphicLayer = loadCSV(graphicLayerPath);
        backgroundLayer = loadCSV(backgroundLayerPath);


        int textureWidth, textureHeight;
        SDL_QueryTexture(tileset, nullptr, nullptr, &textureWidth, &textureHeight);

        // Tính số cột của tileset
        tilesetColumns = (textureWidth - 2 * MARGIN + SPACING) / (TILE_SIZE + SPACING);
        set<int> solidTileIDs;
        ifstream solidFile("assets/map/solid_tiles.txt");

        int id;
        while (solidFile >> id) {
            solidTileIDs.insert(id);
        }

        generateColliders(graphicLayer, solidTileIDs);
    }

    void render(SDL_Renderer* renderer, int cameraX) {
        renderLayer(renderer, backgroundLayer, cameraX);
        renderLayer(renderer, graphicLayer, cameraX);
    }


private:
    int tilesetColumns;

    Manager& manager;

    SDL_Rect src,dest;
    SDL_Renderer *renderer;

    SDL_Texture* tileset;
    vector<vector<int>> graphicLayer;

    vector<vector<int>> backgroundLayer;

    vector<vector<int>> loadCSV(const char* path) {
        vector<vector<int>> data;
        ifstream file(path);
        string line;

        while (getline(file, line)) {
            vector<int> row;
            stringstream ss(line);
            string value;

            while (getline(ss, value, ',')) {
                row.push_back(stoi(value));
            }

            data.push_back(row);
        }

        return data;
    }

    void renderLayer(SDL_Renderer* renderer, const vector<vector<int>>& layer, int cameraX) {
        for (int y = 0; y < MAP_HEIGHT; ++y) {
            for (int x = 0; x < MAP_WIDTH; ++x) {
                int tileID = layer[y][x];
                if (tileID < 0) continue;


                int row = tileID / tilesetColumns;
                int col = tileID % tilesetColumns;

                SDL_Rect srcRect = {
                    MARGIN + col * (TILE_SIZE + SPACING),
                    MARGIN + row * (TILE_SIZE + SPACING),
                    TILE_SIZE, TILE_SIZE
                };

                SDL_Rect destRect = {
                    x * TILE_SIZE * SCALE - cameraX * 4,
                    y * TILE_SIZE * SCALE,
                    TILE_SIZE * SCALE,
                    TILE_SIZE * SCALE
                };

                //SDL_Rect destRect = {x * TILE_SIZE - cameraX, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                //nếu dùng setrenderscale

                SDL_RenderCopy(renderer, tileset, &srcRect, &destRect);

            }
        }
    }

    void generateColliders(const vector<vector<int>>& layer, const set<int>& solidIDs) {
        auto& oldColliders = manager.getGroup(groupColliders);
        for (auto& e : oldColliders) {
            e->destroy();
        }
        manager.refresh();
        for (int y = 0; y < MAP_HEIGHT; ++y) {
            for (int x = 0; x < MAP_WIDTH; ++x) {
                int tileID = layer[y][x];
                if (solidIDs.count(tileID)) {
                    std::cout << "[Collider] Tile ID " << tileID << " at (" << x << "," << y << ")\n";

                    auto& tile = manager.addEntity();
                    tile.addComponent<TransformComponent>(
                        x * TILE_SIZE * SCALE,
                        y * TILE_SIZE * SCALE,
                        TILE_SIZE ,
                        TILE_SIZE ,
                        SCALE
                    );
                    tile.addComponent<ColliderComponent>("solid");
                    tile.addGroup(groupColliders);
                    manager.addToGroup(&tile, groupColliders);

                }
            }
        }
    }
};

#endif // MAP_H_INCLUDED
