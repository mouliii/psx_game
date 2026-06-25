#include "game_scene.h"

void GameScene::start(Scene::StartReason reason)
{
    m_gamePad.initialize(psyqo::AdvancedPad::PollingMode::Normal);
    Graphics::Instance().SetActiveCamera(&camera);
    Graphics::Instance().SetActiveFont(AssetManager::Instance().GetTexture("FONT.TIM;1", gpu()));
    period = 1'000'000;
    timer = gpu().armPeriodicTimer(period, [this](uint32_t) { SpawnNewEnemies(); });
    mouli::random::rngSeed = gpu().now();

    Texture* playerTex = AssetManager::Instance().GetTexture("ARCHER.TIM;1", gpu());
    psyqo::Vertex playerUV = Graphics::Instance().GetTextureUV(playerTex);
    Animation anim{(uint16_t)playerUV.x, (uint16_t)playerUV.y, 16, 16, 4, 5};
    player.Initialize(playerTex, {100,150}, {16,16}, anim, {.health = 100, .damage = 10, .speed = 1.5});
    tilemap = new Tilemap(AssetManager::Instance().GetTexture("TILEMAP.TIM;1", gpu()), AssetManager::Instance().dataStore);
    enemies.reserve(100);
}

// #define MEASURE_PERFORMANCE

void GameScene::frame()
{
    // testaa softmath sqrt vs gte
#ifdef MEASURE_PERFORMANCE
    // We can measure and display the performance of the demo, which is useful when optimizing.
    torusDemo.m_font.chainprintf(gpu(), {{.x = 2, .y = 2}}, {{.r = 0xff, .g = 0xff, .b = 0xff}}, "FPS: %i", gpu().getRefreshRate() / frameDiff);
    // The `now()` function will not return a new value until `pumpCallbacks()` is called, so we need to do that
    // to ensure that we're measuring the time correctly.
    gpu().pumpCallbacks();
    uint32_t endFrame = gpu().now(); <<<
    uint32_t spent = endFrame - beginFrame;
    ramsyscall_printf("Frame took %ius to complete\n", spent);
#endif
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
        delete tilemap;
        break;
    default:
        break;
    }
}

void GameScene::Draw()
{
    Graphics::Instance().DrawText({32,32}, psyqo::Color{255,255,255}, "Hello world \n lol %d\nlul\nxD", 1337);
    static uint8_t rValue = 0;
    Graphics::Instance().DrawText({camera.pos.x - mouli::graphics::SCREEN_WIDTH / 2 + 10, camera.pos.y - mouli::graphics::SCREEN_HEIGHT / 2 + 10}, psyqo::Color{rValue,255,255}, "Fixed text");
    rValue++;
    for (Enemy& e : enemies)
    {
        e.Draw(Graphics::Instance(), 3);
    }
    player.Draw(Graphics::Instance(), 3);
    tilemap->DrawForeground(Graphics::Instance(), &camera, 2);
    tilemap->DrawBackground(Graphics::Instance(), &camera);
    Graphics::Instance().SubmitOT();
}

void GameScene::Update()
{
    player.Update(m_gamePad);
    camera.pos = player.pos;

    for (Enemy& e : enemies)
    {
        e.Update(player.pos);
    }
}

void GameScene::SpawnNewEnemies()
{
    if (enemies.size() >= 100)
    {
        return;
    }
    int16_t randEnemy = mouli::random::GetRandom() % 4;
    Enemy newEnemy;
    Texture* tex = AssetManager::Instance().GetTexture("SPRITES.TIM;1", gpu());
    
    psyqo::Kernel::assert(tex != nullptr, "Texture is null pointer at create new enemy");

    switch (randEnemy)
    {
    case 0:
    {
        // chicken
        psyqo::Vertex texUV = Graphics::Instance().GetTextureUV(tex);
        texUV.y += 16;
        Animation anim{(uint16_t)texUV.x, (uint16_t)texUV.y, 16, 16, 2, 3};
        newEnemy.Initialize(tex, {0,0}, {16,16}, anim, {.health = 35, .damage = 10, .speed = 2});
    }
    break;
    case 1:
    {
        // bat
        psyqo::Vertex texUV = Graphics::Instance().GetTextureUV(tex);
        texUV.y += 3 * 16;
        texUV.x += 8 * 16;
        Animation anim{(uint16_t)texUV.x, (uint16_t)texUV.y, 16, 16, 4, 5};
        newEnemy.Initialize(tex, {0,0}, {16,16}, anim, {.health = 20, .damage = 5, .speed = 3.5});
    }
    break;
    case 2:
    {
        // scarab
        psyqo::Vertex texUV = Graphics::Instance().GetTextureUV(tex);
        texUV.y += 4 * 16;
        texUV.x += 4 * 16;
        Animation anim{(uint16_t)texUV.x, (uint16_t)texUV.y, 16, 16, 4, 5};
        newEnemy.Initialize(tex, {0,0}, {16,16}, anim, {.health = 65, .damage = 10, .speed = 1});
    }
    break;
    case 3:
    {
        // werewolf
        psyqo::Vertex texUV = Graphics::Instance().GetTextureUV(tex);
        texUV.y += 1 * 16;
        texUV.x += 8 * 16;
        Animation anim{(uint16_t)texUV.x, (uint16_t)texUV.y, 16, 16, 4, 5};
        newEnemy.Initialize(tex, {0,0}, {16,16}, anim, {.health = 80, .damage = 15, .speed = 1.5});
    }
    default:
        break;
    }
    // randomize which side then offset
    // or circle
    int32_t fixedNumber = (mouli::random::GetRandom() % 4096) * 2 - 4096;
    int32_t wholePart = fixedNumber >> 12;
    int32_t fracPart = fixedNumber & 0xfff;
    psyqo::FixedPoint<> pi(wholePart, fracPart);
    psyqo::Angle angle(pi);
    psyqo::Vec2 pos;
    pos.x = trig.cos(angle) * mouli::graphics::SCREEN_WIDTH / 2;
    pos.y = trig.sin(angle) * mouli::graphics::SCREEN_WIDTH / 2;
    pos += player.pos;
    newEnemy.SetPosition(pos);
    enemies.push_back(newEnemy);
}


