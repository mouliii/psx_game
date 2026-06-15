#include "tilemap.h"
#include "EASTL/algorithm.h"

Tilemap::Tilemap(Texture* tex, psyqo::Buffer<uint8_t>& tilemapData)
    :
    tileSheet(tex)
{
    uint8_t* pHelper = tilemapData.begin();
    if (*pHelper != TILEMAP_ID)
    {
        syscall_puts("invalid TILEMAP file. ID is not 125. Aborting...\n");
        printf("ID was: %d\n", *pHelper);
        return;
    }
    pHelper++;
    // version
    pHelper++;
    tileSize = *pHelper++;
    mapWidth = *pHelper++;
    mapHeight = *pHelper++;
    uint32_t nTiles = mapWidth * mapHeight;
    tiles.reserve(nTiles);
    for (uint32_t i = 0; i < nTiles; i++)
    {
        tiles.emplace_back(*pHelper++);
    }
}

Tilemap::~Tilemap()
{
    tiles.clear();
}

void Tilemap::Draw(Graphics &gfx, Camera2D* cam, int layer)
{
    int16_t tileSheetWidth = tileSheet->width / tileSize;
    int16_t tileSheetHeight = tileSheet->height / tileSize;
    psyqo::Rect tileUV = {{0,0},{(int16_t)(tileSize - 1), (int16_t)(tileSize - 1)}};
    int16_t tilePosX, tilePosY;

    if (cam)
    {
        //TODO: hardcoded values
        int16_t topLeftX = int16_t(((cam->pos.x.integer() - cam->CAMERA_OFFSET.x) / tileSize) - 1);
        int16_t topLeftY = int16_t(((cam->pos.y.integer() - cam->CAMERA_OFFSET.y) / tileSize) - 1);
        int16_t topRightX = eastl::clamp(int16_t(topLeftX   + (320/16) + 2), (int16_t)0, int16_t(mapWidth));
        int16_t bottomLeftY = eastl::clamp(int16_t(topLeftY + (240/16) + 2), (int16_t)0, int16_t(mapHeight));
        topLeftX = eastl::max((int16_t)0, topLeftX);
        topLeftY = eastl::max((int16_t)0, topLeftY);

        //printf("TL:%d %d, TR:%d, BL:%d\n", topLeftX, topLeftY, topRightX, bottomLeftY);
        //printf("CAM: %d, %d\n", cam->pos.x.integer(), cam->pos.y.integer());

        for (size_t y = topLeftY; y < bottomLeftY; y++)
        {
            for (size_t x = topLeftX; x < topRightX; x++)
            {
                const uint16_t tileIndex = tiles[y * mapWidth + x];
                tileUV.pos.x = (tileIndex % tileSheetWidth) * tileSize;
                tileUV.pos.y = (tileIndex / tileSheetWidth) * tileSize;

                psyqo::Vec2 tilePos{mapPosition.x + (x * tileSize), mapPosition.y + (y * tileSize)};

                gfx.DrawSprite16x16(tileSheet, tilePos, tileUV, layer);
            }
        }
    }
    else
    {// draw whole tilemap
        for (size_t i = 0; i < tiles.size(); i++)
        {
            tilePosX = (i % mapWidth) * tileSize;
            tilePosY = (i / mapWidth) * tileSize;
            if (tiles[i] != 255)
            {
                psyqo::Vec2 tilePos{mapPosition.x + tilePosX, mapPosition.y + tilePosY};

                const uint8_t tileID = tiles[i];
                tileUV.pos.x = (tileID % tileSheetWidth) * tileSize;
                tileUV.pos.y = (tileID / tileSheetWidth) * tileSize;

                //gfx.DrawTexturedQuad(tileSheet, tilePos, {(int16_t)tileSize, (int16_t)tileSize}, tileUV, layer);
                gfx.DrawSprite16x16(tileSheet, tilePos, tileUV, layer);
            }
        }
    }
    gfx.SetTpage(tileSheet, layer);
}
