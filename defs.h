#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 624;
const char* WINDOW_TITLE = "Mario";




//Map
const int MAP_WIDTH = 240;
const int MAP_HEIGHT = 13;
const int TILE_SIZE = 16;
const int SPACING = 2;
const int MARGIN = 1;
const int TILE_COLS = 16;
const int SCALE = 3; //phóng to map

const int FPS = 60;
const int FrameDelay = 1000/ FPS;


enum GroupLabels : std::size_t {
    groupMap,
    groupPlayers,
    groupEnemies,
    groupColliders,
};

#endif // DEFS_H_INCLUDED
