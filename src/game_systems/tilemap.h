#pragma once

#include "EASTL/vector.h"
#include "psyqo/buffer.hh"
#include "../core_systems/graphics.h"
#include "../core_systems/asset_manager.h"
#include "camera.h"

class Tilemap
{
public:
    Tilemap(Texture* tex, psyqo::Buffer<uint8_t>& tilemapData);
    ~Tilemap();
    void Draw(Graphics& gfx, Camera2D* cam, int layer = 5); // draw sisälle temp psyqo::Rect?, miten alue?
    Texture* tileSheet;
private:
    static constexpr uint8_t TILEMAP_ID = 125;
    psyqo::Vec2 mapPosition{0,0}; // TODO
    uint16_t mapWidth;
    uint16_t mapHeight;
    uint16_t tileSize;
    eastl::vector<int> tiles;
};