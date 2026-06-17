
#include "main_menu.h"
#include "psyqo/alloc.h"
#include "EASTL/memory.h"

void MainMenuScene::start(Scene::StartReason reason)
{
    switch (reason)
    {
        case StartReason::Create:
        {
            Graphics::Instance().SetActiveCamera(&camera);
            m_systemFont.uploadSystemFont(gpu());
            m_gamePad.initialize(psyqo::AdvancedPad::PollingMode::Normal);
            gameScene = new GameScene();
            Graphics::Instance().SubmitOT();
            Graphics::Instance().EndFrame();
            pushScene(gameScene);
        }
        break;
        case StartReason::Resume:
        break;
    default:
        break;
    }
}

void MainMenuScene::frame()
{
    Graphics::Instance().BeginFrame();
    Update();
    Draw();
    Graphics::Instance().EndFrame();
}

void MainMenuScene::teardown(Scene::TearDownReason reason)
{
}


void MainMenuScene::Draw()
{
    Graphics::Instance().DrawTexturedQuad(AssetManager::Instance().GetTexture("BGLEFT.TIM;1", gpu()),  {0,0},   {160,240}, {{0,0},{160,240}}, 4);
    Graphics::Instance().DrawTexturedQuad(AssetManager::Instance().GetTexture("BGRIGHT.TIM;1", gpu()), {160,0}, {160,240}, {{0,0},{160,240}}, 4);
    Graphics::Instance().SubmitOT();
}

void MainMenuScene::Update()
{
}
