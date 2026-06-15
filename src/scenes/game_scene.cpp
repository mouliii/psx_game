#include "game_scene.h"

void GameScene::start(Scene::StartReason reason)
{
    Graphics::Instance().SetActiveCamera(&camera);
    player = new Player(AssetManager::Instance().GetTexture("ARCHER.TIM;1", gpu()), {100,150}, {16,16});
    tilemap = new Tilemap(AssetManager::Instance().GetTexture("TILEMAP.TIM;1", gpu()), AssetManager::Instance().dataStore);
}

void GameScene::frame()
{
}

void GameScene::teardown(Scene::TearDownReason reason)
{
    delete player;
    delete tilemap;
}

void GameScene::Draw()
{
}

void GameScene::Update()
{
    player->Update(m_gamePad);
    camera.pos = player->pos;
}
