#include "loading_screen.h"
#include "EASTL/vector.h"
#include "psyqo/xprintf.h"

eastl::string txt("0", 32);
eastl::string loadDone("Loading...", 32);
eastl::vector<eastl::string_view> ass{"AWESOME.TIM;1", "BGLEFT.TIM;1", "BGRIGHT.TIM;1", "ARCHER.TIM;1", "TILEMAP.TIM;1", "MAP1.MAP;1"};

void LoadingScreenScene::start(Scene::StartReason reason)
{
    syscall_puts("loading screen start()\n");
    m_systemFont.uploadSystemFont(gpu());

    //*** EI MENE TEX NIMI LÄPI ***

    //m_texCoro = AssetManager::Instance().LoadTexture("AWESOME.TIM;1", gpu());
    m_loaderCoro = AssetManager::Instance().LoadLevel(ass, gpu());
    m_loaderCoro.resume();
}

uint32_t frameCounter = 0;
void LoadingScreenScene::frame()
{
    Graphics::Instance().BeginFrame();

    txt = sprintf("Frame: %d", frameCounter);
    m_systemFont.chainprint(gpu(), "testitesti", {16, 32}, {255,255,255});

    frameCounter++;

    if (m_loaderCoro.done())
    {
        loadDone = "DONE !";
        //popScene();
        pushScene(&menu);
    }
    m_systemFont.chainprint(gpu(), loadDone.c_str(), {16, 64}, {255,255,255});
    Graphics::Instance().EndFrame();
}

void LoadingScreenScene::teardown(Scene::TearDownReason reason)
{
}
