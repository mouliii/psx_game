#pragma once

#include "EASTL/vector.h"
#include "psyqo/buffer.hh"
#include "../core_systems/graphics.h"
#include "../core_systems/asset_manager.h"
#include "camera.h"

class Tilemap
{
struct ForegroundTile;
public:
    Tilemap(Texture* tex, psyqo::Buffer<uint8_t>& tilemapData);
    ~Tilemap();
    void DrawBackground(Graphics& gfx, Camera2D* cam, int layer = 5);
    void DrawForeground(Graphics& gfx, Camera2D* cam, int layer = 5);
    Texture* tileSheet;
private:
    static constexpr uint8_t TILEMAP_ID = 125;
    static constexpr uint8_t TM_VERSION = 3;
    psyqo::Vec2 mapPosition{0,0}; // TODO, mapOffset ?
    uint16_t mapWidth;
    uint16_t mapHeight;
    uint16_t tileSize;
    eastl::vector<uint8_t> tiles;
    eastl::vector<ForegroundTile> fgTiles;
    
    struct ForegroundTile
    {
        uint8_t x, y;
        uint8_t tileID;
    };
public:
};