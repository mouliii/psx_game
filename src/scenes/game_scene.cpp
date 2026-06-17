#include "game_scene.h"

void GameScene::start(Scene::StartReason reason)
{
    m_gamePad.initialize(psyqo::AdvancedPad::PollingMode::Normal);
    Graphics::Instance().SetActiveCamera(&camera);
    player = new Player(AssetManager::Instance().GetTexture("ARCHER.TIM;1", gpu()), {100,150}, {16,16});
    tilemap = new Tilemap(AssetManager::Instance().GetTexture("TILEMAP.TIM;1", gpu()), AssetManager::Instance().dataStore);
}

void GameScene::frame()
{
    Graphics::Instance().BeginFrame();
    Update();
    Draw();
    Graphics::Instance().EndFrame();
}

void GameScene::teardown(Scene::TearDownReason reason)
{
    switch (reason)
    {
    case TearDownReason::Pause:
    
        break;
    case TearDownReason::Destroy:
        delete player;
        delete tilemap;
        break;
    default:
        break;
    }
}

void GameScene::Draw()
{
    player->Draw(Graphics::Instance(), 3);
    tilemap->DrawForeground(Graphics::Instance(), &camera, 2);
    tilemap->DrawBackground(Graphics::Instance(), &camera);
    Graphics::Instance().SubmitOT();
}

void GameScene::Update()
{
    player->Update(m_gamePad);
    camera.pos = player->pos;
}
