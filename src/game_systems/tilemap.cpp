#include "tilemap.h"
#include "EASTL/algorithm.h"
#include "psyqo/kernel.hh"

Tilemap::Tilemap(Texture* tex, psyqo::Buffer<uint8_t>& tilemapData)
    :
    tileSheet(tex)
{
    psyqo::Kernel::assert(tex, "Texture is null!");
    uint8_t* pHelper = tilemapData.begin();
    if (*pHelper != TILEMAP_ID)
    {
        syscall_puts("invalid TILEMAP file. ID is not 125. Aborting...\n");
        printf("ID was: %d\n", *pHelper);
        psyqo::Kernel::assert(*pHelper == TILEMAP_ID, "version mismatch\n");
        return;
    }
    pHelper++;
    if (*pHelper != TM_VERSION)
    {
        printf("wrong version! needs to be %d\n, got %d", TM_VERSION, *pHelper);
        psyqo::Kernel::assert(*pHelper == TM_VERSION, "version mismatch\n");
        return;
    }
    
    pHelper++;
    tileSize = *pHelper++;
    mapWidth = *pHelper++;
    mapHeight = *pHelper++;
    uint8_t nLayers = *pHelper++;
    uint32_t nTiles = mapWidth * mapHeight;
    tiles.reserve(nTiles);
    // BACKGROUND TILES
    for (uint32_t i = 0; i < nTiles; i++)
    {
        tiles.emplace_back(*pHelper++);
    }
    // FOREGROUND TILES
    if (nLayers > 1)
    {
        uint16_t nTiles = *(uint16_t*)pHelper;
        pHelper += sizeof(uint16_t);
        if (*pHelper != 0xff)
        {
            psyqo::Kernel::assert(*pHelper == 0xff, "Foreground tile sync fuq'd up\n");
            return;
        }
        pHelper++;
        for (size_t i = 0; i < nTiles; i++)
        {
            fgTiles.emplace_back(ForegroundTile{.x = pHelper[0], .y =pHelper[1], .tileID = pHelper[2]});
            pHelper += sizeof(ForegroundTile);
        }
        // TODO multilayer
    }
    
}

Tilemap::~Tilemap()
{
    tiles.clear();
}

void Tilemap::DrawBackground(Graphics &gfx, Camera2D* cam, int layer)
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
        int16_t topRightX = eastl::clamp(int16_t(topLeftX   + (mouli::graphics::SCREEN_WIDTH / tileSize) + 2), (int16_t)0, int16_t(mapWidth));
        int16_t bottomLeftY = eastl::clamp(int16_t(topLeftY + (mouli::graphics::SCREEN_HEIGHT / tileSize) + 2), (int16_t)0, int16_t(mapHeight));
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

void Tilemap::DrawForeground(Graphics &gfx, Camera2D *cam, int layer)
{
    psyqo::Rect tileUV = {{0,0},{(int16_t)(tileSize - 1), (int16_t)(tileSize - 1)}};
    int16_t tileSheetWidth = tileSheet->width / tileSize;
    for (auto& tile : fgTiles)
    {
        psyqo::Vec2 tilePos
        {
            mapPosition.x + tile.x * tileSize,
            mapPosition.y + tile.y * tileSize
        };
        // TODO: AABB
        const int16_t halfW = cam->CAMERA_OFFSET.x;
        const int16_t halfH = cam->CAMERA_OFFSET.y;

        const int16_t camCenterX = cam->pos.x.integer();
        const int16_t camCenterY = cam->pos.y.integer();

        const int16_t camLeft   = camCenterX - halfW;
        const int16_t camRight  = camCenterX + halfW;
        const int16_t camTop    = camCenterY - halfH;
        const int16_t camBottom = camCenterY + halfH;

        const int16_t tileLeft   = tilePos.x.integer();
        const int16_t tileTop    = tilePos.y.integer();
        const int16_t tileRight  = tileLeft + tileSize;
        const int16_t tileBottom = tileTop  + tileSize;
        //printf("T: %d,%d | cam: %d,%d\n", tileLeft, tileTop, camLeft, camRight);
        if (tileRight > camLeft &&
            tileLeft < camRight &&
            tileBottom > camTop &&
            tileTop < camBottom)
        {
            tileUV.pos.x = (tile.tileID % tileSheetWidth) * tileSize;
            tileUV.pos.y = (tile.tileID / tileSheetWidth) * tileSize;

            gfx.DrawSprite16x16(tileSheet, tilePos, tileUV);
        }
    }
    gfx.SetTpage(tileSheet, layer);
}
