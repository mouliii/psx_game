#include "asset_manager.h"
#include "cdrom_loader.h"
#include "psyqo/alloc.h"
#include "EASTL/string.h"

AssetManager* AssetManager::m_instance;

void AssetManager::Initialize()
{
    if (m_instance == nullptr) 
    {
        m_instance = new AssetManager();
    }
    AssetManager::Instance().textures.reserve(16);
    AssetManager::Instance().taskQue.startWith([](auto task){task->resolve();});
}

psyqo::Coroutine<> AssetManager::LoadFile(eastl::string_view filePath, eastl::function<void(psyqo::Buffer<uint8_t> &&)> callBack)
{
    psyqo::Buffer<uint8_t> data = co_await CdRomLoader::Instance().LoadFile(filePath);
    callBack(eastl::move(data));
}


psyqo::Coroutine<Texture*> AssetManager::LoadTexture(eastl::string_view filePath, psyqo::GPU &gpu)
{
    // check
    size_t nPos = filePath.find_last_of('/'); // , 6? .TIM;1
    eastl::string_view sv = filePath.substr(nPos + 1).data();
    AssetStr fileName(sv.begin(), sv.size());
    for (size_t i = 0; i < textures.size(); i++)
    {
        if(textures[i].name == fileName)
        {
            co_return &textures[i];
        }
    }
    
    // load
    // check empty
    // tim parse
    // uplaod
    // Tex* newTex
    // tex säätö
    // store
    
    psyqo::Buffer<uint8_t> data = co_await CdRomLoader::Instance().LoadFile(filePath);
    if (data.empty()){
        syscall_puts("NO DATA READ! *FAILED*\n");
        co_return nullptr;
    }
    Texture tex;
    tex.name = fileName;
    ParseAndUploadTim(data, gpu, tex);
    textures.push_back(tex);

    co_return &textures.back();
}

psyqo::Coroutine<> AssetManager::LoadTilemap(eastl::string_view filePath)
{
    psyqo::Buffer<uint8_t> data = co_await CdRomLoader::Instance().LoadFile(filePath);
    if (data.empty()){
        syscall_puts("NO DATA READ! *FAILED*\n");
        //co_return nullptr;
    }
    dataStore = eastl::move(data);
    co_return;
}

psyqo::Coroutine<> AssetManager::LoadLevel(eastl::string_view levelFilePath, psyqo::GPU &gpu)
{
    // load level.dat
    psyqo::Buffer<uint8_t> levelFile = co_await CdRomLoader::Instance().LoadFile(levelFilePath);
    // get all required files list
    eastl::vector<eastl::string_view> filesToLoad; // = Parse();
    // load them
    for (const auto& filePath : filesToLoad) 
    {
        // HUOM. ;1
        if (filePath.ends_with(".TIM;1")) {
            co_await LoadTexture(filePath, gpu);
        } else if (filePath.ends_with(".VAG")) {
            //co_await LoadSound(filePath);
        }
        // update loading screen animation...
    }
    
    co_return;
}

psyqo::Coroutine<> AssetManager::LoadLevel(eastl::vector<eastl::string_view> fileNames, psyqo::GPU &gpu)
{
    // load level.dat
    //psyqo::Buffer<uint8_t> levelFile = co_await CdRomLoader::Instance().LoadFile(levelFilePath);
    // get all required files list
    eastl::vector<eastl::string_view> filesToLoad = fileNames; // = Parse();
    // load them
    for (size_t i = 0; i < fileNames.size(); i++) 
    {
        // HUOM. ;1
        if (fileNames[i].ends_with(".TIM;1")) 
        {
            co_await LoadTexture(fileNames[i], gpu);
        }
        else if (fileNames[i].ends_with(".VAG;1")) 
        {
            //co_await LoadSound(filePath);
        }
        else if (fileNames[i].ends_with(".MAP;1"))
        {
            co_await LoadTilemap(fileNames[i]);
        }
    }
    co_return;
}

bool AssetManager::ParseAndUploadTim(psyqo::Buffer<uint8_t> buffer, psyqo::GPU& gpu, Texture& tex)
{
    // make pointers to buffer and pass to vram upload
    TIM_File tim; // TODO: <<<<<<<<<<<<<<<
    uint32_t* pHelper;
    pHelper = (uint32_t*)buffer.begin();

    if ( (*pHelper & 0xff) != 0x10){
        syscall_puts("invalid .TIM file. ID is not 0x10. Aborting...\n");
        return false;
    }
    pHelper++;
    // color mode
    switch (*pHelper & 0b11)
    {
    case 0:
        tex.colorMode = psyqo::Prim::TPageAttr::ColorMode::Tex4Bits;
        break;
    case 1:
        tex.colorMode = psyqo::Prim::TPageAttr::ColorMode::Tex8Bits;
        break;
    case 2:
        tex.colorMode = psyqo::Prim::TPageAttr::ColorMode::Tex16Bits;
        break;
    default:
        syscall_puts("TEXTURE: UNKOWN COLOR MODE (or 24bpp)\n");
        break;
    }
    tex.hasClut = *pHelper & (1 << 3);
    pHelper++;
    if (tex.hasClut)
    {
        // CLUT
        tim.clut.clutSize = *pHelper;
        tex.clutX = *(pHelper + 1) & 0xffff;
        tex.clutY = *(pHelper + 1) >> 16;
        tex.clutWidth = *(pHelper + 2) & 0xffff;
        tex.clutHeight = *(pHelper + 2) >> 16;
        tim.clut.pClutData = (uint16_t*)(pHelper + 3);
        pHelper += tim.clut.clutSize / 4;
    }
    
    // IMAGE
    tim.image.imgSize = *pHelper;
    tex.x = static_cast<int16_t>(*(pHelper + 1) & 0xffff);
    tex.y = static_cast<int16_t>(*(pHelper + 1) >> 16);
    tex.width = static_cast<int16_t>(*(pHelper + 2) & 0xffff) * (1 << (2 - tex.colorMode));
    tex.height = static_cast<int16_t>(*(pHelper + 2) >> 16);
    uint16_t vramWidth = static_cast<int16_t>(*(pHelper + 2) & 0xffff);
    uint16_t vramHeight = static_cast<int16_t>(*(pHelper + 2) >> 16);
    tim.image.pImgData = (uint16_t*)(pHelper + 3);

    // texture
    gpu.uploadToVRAM(tim.image.pImgData, psyqo::Rect{.pos={(int16_t)tex.x, (int16_t)tex.y}, .size={(int16_t)vramWidth,(int16_t)vramHeight}});
    // clut
    if (tex.hasClut)
    {
        gpu.uploadToVRAM(tim.clut.pClutData, psyqo::Rect{.pos={(int16_t)tex.clutX, (int16_t)tex.clutY}, .size={(int16_t)tex.clutWidth,(int16_t)tex.clutHeight}});
    }
    

    return true;
}

Texture* AssetManager::GetTexture(eastl::string_view fileName, psyqo::GPU &gpu)
{
    for (size_t i = 0; i < textures.size(); i++)
    {
        if (textures[i].name == fileName)
        {
            return &textures[i];
        }
    }
    
    return nullptr;
}

void AssetManager::StoreTexture(eastl::string_view fileName, Texture tex)
{
    syscall_puts("STORED NEW TEXTURE\n");
    //m_assets[AssetStr(fileName.data(), MAX_FILENAME_LEN)] = tex;
    textures.emplace_back(tex);
}