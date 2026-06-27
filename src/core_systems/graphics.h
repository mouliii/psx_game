    #pragma once
    #include "common/syscalls/syscalls.h"
    #include "psyqo/application.hh"
    #include "psyqo/font.hh"
    #include "psyqo/gpu.hh"
    #include "psyqo/scene.hh"
    #include "psyqo/coroutine.hh"
    #include "psyqo/primitives.hh"
    #include "psyqo/task.hh"
    #include "psyqo/fragments.hh"
    #include "psyqo/ordering-table.hh"
    #include "psyqo/bump-allocator.hh"
    #include "asset_manager.h"
    #include "../game_systems/camera.h"

    //#include "../player/player.h"
// INFO: psyqo::Fragments::FixedFragment<psyqo::Prim::Sprite16x16, MAX_TILES>


class Camera2D;
namespace mouli::graphics
{
static constexpr uint16_t SCREEN_WIDTH = 320;
static constexpr uint16_t SCREEN_HEIGHT = 240;
}

class Graphics 
{
public:
    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;

    static void Initialize(psyqo::GPU& gpu);
    static Graphics& Instance() { return *m_instance; }
    psyqo::GPU& GPU() { return m_gpu; }

    void BeginFrame();
    void EndFrame();
    void SubmitOT();
    void SetClearColor(psyqo::Color color);
    void SetActiveCamera(Camera2D* camera);
    void SetActiveFont(Texture* tex);
    const Camera2D* ActiveCamera() const { return m_activeCamera; }
    //void UploadFont()
    void DrawText(const psyqo::Vec2 pos, const psyqo::Color color, const char* format, ...);
    psyqo::Vertex GetTextureUV(Texture* texture);
    void DrawTexturedQuad(const Texture* texture, const psyqo::Vec2 pos, const psyqo::Vertex size, const psyqo::Rect uv, uint16_t otEntry = 0, psyqo::Color color = {.r = 128, .g = 128, .b = 128});
    void SetTpage(const Texture* texture, uint16_t otEntry);
    // Sprites do not include TPage!
    void DrawSprite16x16(const Texture* texture, const psyqo::Vec2 pos, const psyqo::Rect uv, uint16_t otEntry = 0, psyqo::Color color = {.r = 128, .g = 128, .b = 128});
    void DrawLine(const psyqo::Vec2 start, const psyqo::Vec2 end, psyqo::Color color = {.r = 255, .g = 255, .b = 255}, uint16_t otEntry = 0);
    void DrawCircle(const psyqo::Vec2 pos, const uint16_t radius, const uint16_t segments, psyqo::Color color = {.r = 255, .g = 255, .b = 255}, uint16_t otEntry = 0);
private:
    Graphics(psyqo::GPU& gpu);
    ~Graphics() = default;
    static Graphics* m_instance;

    psyqo::GPU& m_gpu;
    Camera2D* m_activeCamera = nullptr;
    Texture* fontTexture;

    static constexpr uint16_t OT_SIZE = 10;
    static constexpr uint32_t PRIM_BUF_SIZE = 400000;

    psyqo::OrderingTable<OT_SIZE> m_orderingTables[2];
    psyqo::BumpAllocator<PRIM_BUF_SIZE> m_primitiveBuffers[2];
    unsigned int m_parity = 0;

    psyqo::Color clearColor = {52, 73, 235};

    static constexpr uint16_t TPAGE_WIDTH = 64;
    static constexpr uint16_t TPAGE_HEIGHT = 256;

};