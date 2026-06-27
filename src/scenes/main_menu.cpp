
#include "main_menu.h"
#include "psyqo/alloc.h"
#include "EASTL/memory.h"

void MainMenuScene::start(Scene::StartReason reason)
{
    switch (reason)
    {
        case StartReason::Create:
        {
            //Graphics::Instance().SetActiveFont(AssetManager::Instance().GetTexture("FONT.TIM;1", gpu()));
            Graphics::Instance().SetActiveCamera(&camera);
            Gamepad::Instance().GetGamepad().setOnEvent([this](psyqo::AdvancedPad::Event event)
            {
                ButtonEvents(event);
            });
        }
        break;
        case StartReason::Resume:
        {
            // TODO: not work if quitting to main menu from game
            gameScene = new GameScene();
            Graphics::Instance().EndFrame();
            pushScene(gameScene);
        }
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
    const psyqo::Vec2 bannerPos = {63,50};
    // banner left
    Graphics::Instance().DrawTexturedQuad(AssetManager::Instance().GetTexture("SPRITES.TIM;1", gpu()), {bannerPos.x, bannerPos.y}, {32,32}, {{0,6*16},{32,32}}, 3);
    // banner mid
    Graphics::Instance().DrawTexturedQuad(AssetManager::Instance().GetTexture("SPRITES.TIM;1", gpu()), {bannerPos.x + 32, bannerPos.y}, {32,32}, {{2*16,6*16},{32,32}}, 3);
    Graphics::Instance().DrawTexturedQuad(AssetManager::Instance().GetTexture("SPRITES.TIM;1", gpu()), {bannerPos.x + 64, bannerPos.y}, {32,32}, {{2*16,6*16},{32,32}}, 3);
    Graphics::Instance().DrawTexturedQuad(AssetManager::Instance().GetTexture("SPRITES.TIM;1", gpu()), {bannerPos.x + 96, bannerPos.y}, {32,32}, {{2*16,6*16},{32,32}}, 3);
    Graphics::Instance().DrawTexturedQuad(AssetManager::Instance().GetTexture("SPRITES.TIM;1", gpu()), {bannerPos.x + 128, bannerPos.y}, {32,32}, {{2*16,6*16},{32,32}}, 3);
    // banner right
    Graphics::Instance().DrawTexturedQuad(AssetManager::Instance().GetTexture("SPRITES.TIM;1", gpu()), {bannerPos.x + 160, bannerPos.y}, {32,32}, {{4*16,6*16},{32,32}}, 3);
    // game name
    Graphics::Instance().DrawText({bannerPos.x + 24, bannerPos.y + 8}, {255,255,255}, "GAME NAME: The game");

    auto* tex = AssetManager::Instance().GetTexture("AWESOME.TIM;1", gpu());
    auto uv = Graphics::Instance().GetTextureUV(tex);
    Graphics::Instance().DrawTexturedQuad(tex, selectorPos, {16,16}, {uv,{64,64}}, 3);
    Graphics::Instance().DrawText({firstRowPos.x + 20, firstRowPos.y},      {20,5,255}, "Play");
    Graphics::Instance().DrawText({firstRowPos.x + 20, firstRowPos.y + 16}, {20,5,255}, "Settings");
    Graphics::Instance().DrawText({firstRowPos.x + 20, firstRowPos.y + 32}, {20,5,255}, "\\_(-.-)_/");
    
}

void MainMenuScene::Update()
{
    
}

void MainMenuScene::ButtonEvents(const psyqo::AdvancedPad::Event& event)
{
    switch (event.type)
    {
    case psyqo::AdvancedPad::Event::ButtonPressed:
        switch (event.button)
        {
            case psyqo::AdvancedPad::Cross:
                if (menuIndex == 0)
                {
                    Gamepad::Instance().GetGamepad().setOnEvent(nullptr);
                    AssetManager::Instance().filesToLoad = {"ARCHER.TIM;1", "TILEMAP.TIM;1", "MAP1.MAP;1", "SPRITES.TIM;1"};
                    loadScene = new LoadingScreenScene();
                    pushScene(loadScene);
                }
            break;

            case psyqo::AdvancedPad::Down:
                selectorPos.y += 16;
                if (++menuIndex > 2)
                {
                    menuIndex = 0;
                    selectorPos.y = firstRowPos.y;
                }
            break;

            case psyqo::AdvancedPad::Up:
                selectorPos.y -= 16;
                if (--menuIndex < 0)
                {
                    menuIndex = 2;
                    selectorPos.y = firstRowPos.y + 2 * 16;
                }
            break;
        }
        break;
    break;
    case psyqo::AdvancedPad::Event::ButtonReleased:
        
        break;
    case psyqo::AdvancedPad::Event::PadConnected:
        
        break;
    case psyqo::AdvancedPad::Event::PadDisconnected:
        
        break;
    
    default:
        break;
    }
    
}
