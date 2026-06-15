#pragma once

#include "psyqo/primitives/common.hh"
#include "psyqo/coroutine.hh"
#include "psyqo/gpu.hh"
#include "psyqo/task.hh"
#include "psyqo/buffer.hh"
#include "EASTL/unordered_map.h"
#include "EASTL/array.h"
#include "EASTL/functional.h"
#include "EASTL/vector.h"
#include "tim_file.h"

constexpr int MAX_FILENAME_LEN = 15;
using AssetStr = eastl::fixed_string<char, MAX_FILENAME_LEN, false>;

struct Asset
{
    // 12345678.TIM;1 + \0 = 15
    AssetStr name;
};

struct Texture : public Asset
{
    uint16_t x, y, width, height;
    psyqo::Prim::TPageAttr::ColorMode colorMode;
    uint16_t clutX, clutY;          
    uint16_t clutWidth, clutHeight;
    bool hasClut;
};

class AssetManager
{
public:
    static AssetManager& Instance() { return *m_instance; }
    static void Initialize();
    psyqo::Coroutine<> LoadFile(eastl::string_view filePath, eastl::function<void(psyqo::Buffer<uint8_t>&& data)> callBack);
    psyqo::Coroutine<Texture*> LoadTexture(eastl::string_view filePath, psyqo::GPU& gpu);
    psyqo::Coroutine<> LoadTilemap(eastl::string_view filePath);
    // TODO  // "level1.dat"
    psyqo::Coroutine<> LoadLevel(eastl::string_view levelFilePath, psyqo::GPU& gpu);
    psyqo::Coroutine<> LoadLevel(eastl::vector<eastl::string_view> fileNames, psyqo::GPU& gpu);
    // gpu
    bool ParseAndUploadTim(psyqo::Buffer<uint8_t> buffer, psyqo::GPU& gpu, Texture& tex);
    Texture* GetTexture(eastl::string_view fileName, psyqo::GPU& gpu);
    // load and store pointers then std::move ?
    //void DatastoreAllocate(uint32_t size);
    //void DatastoreCopyTo(uint8_t* src, uint8_t* dst, size_t size);
    //void DatastoreDelete();
public:
    psyqo::Buffer<uint8_t> dataStore;
private:
    void StoreTexture(eastl::string_view fileName, Texture tex);
private:
    AssetManager() = default;
    static AssetManager* m_instance;
    psyqo::Coroutine<> m_CDRomCoro;
    psyqo::TaskQueue taskQue;
    psyqo::Coroutine<Texture*> m_texCoro;
    eastl::vector<Texture> textures;
    eastl::unordered_map<AssetStr, psyqo::Buffer<uint8_t>> m_assets;
    // Tilemap data
    //eastl::unordered_map<AssetStr, Texture> m_textures;
    //eastl::array<Texture,16> m_textArr;
};