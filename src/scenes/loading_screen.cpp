#include "loading_screen.h"
#include "EASTL/vector.h"
#include "psyqo/xprintf.h"

eastl::string txt("0", 32);
eastl::string loadDone("Loading...", 32);

void LoadingScreenScene::start(Scene::StartReason reason)
{
    //syscall_puts("loading screen start()\n");

    //m_texCoro = AssetManager::Instance().LoadTexture("AWESOME.TIM;1", gpu());
    m_loaderCoro = AssetManager::Instance().LoadLevel(AssetManager::Instance().filesToLoad, gpu());
    m_loaderCoro.resume();
}

uint32_t frameCounter = 0;
void LoadingScreenScene::frame()
{
    Graphics::Instance().BeginFrame();

    Graphics::Instance().DrawText({16, 32}, {255,255,255}, "Frame: %d", frameCounter);
    frameCounter++;
    if (m_loaderCoro.done())
    {
        loadDone = "DONE !";
        Graphics::Instance().EndFrame();
        popScene();
        return;
    }
    Graphics::Instance().DrawText({16, 64}, {255,255,255}, "Loading...");
    
    Graphics::Instance().EndFrame();
}

void LoadingScreenScene::teardown(Scene::TearDownReason reason)
{
    AssetManager::Instance().filesToLoad.clear();
}
