
#include "main_menu.h"
#include "psyqo/alloc.h"
#include "EASTL/memory.h"

MainMenuScene::MainMenuScene()
    : mainMenu("root"), subMenu("Sub menu")
{
    mainMenu.pos = {95+20,120};
    mainMenu.AddItem("Play", [this](){StartGame();}, nullptr);
    mainMenu.AddItem("Sub menu", {}, &subMenu);
    subMenu.pos = {95+20,120};
    subMenu.AddItem("Controller test", {}, nullptr);
    subMenu.AddItem("Sub menu 2", {}, nullptr);
    menuStack.push_back(&mainMenu);
}

void MainMenuScene::start(Scene::StartReason reason)
{
    selectorPos = {95,120};
    firstRowPos = {95+20,120};
    menuIndex = 0;
    nMenuElements = 2;
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
    Gamepad::Instance().GetGamepad().setOnEvent(nullptr);
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
    selectorPos = menuStack.back()->GetSelectedPosition();
    selectorPos.x -= 20;
    Graphics::Instance().DrawTexturedQuad(tex, selectorPos, {16,16}, {uv,{64,64}}, 3);

    menuStack.back()->Draw();
}

void MainMenuScene::Update()
{
}

void MainMenuScene::StartGame()
{
    AssetManager::Instance().filesToLoad = {"ARCHER.TIM;1", "TILEMAP.TIM;1", "MAP1.MAP;1", "SPRITES.TIM;1"};
    loadScene = new LoadingScreenScene();
    pushScene(loadScene);
}

void MainMenuScene::ButtonEvents(const psyqo::AdvancedPad::Event& event)
{
    auto action = menuStack.back()->Update(event);
    switch (action)
    {
    case MenuAction::ACTION:
        // executed in menu object
        break;
    case MenuAction::PUSH:
        menuStack.push_back(menuStack.back()->GetSelectedMenu());
        break;
    case MenuAction::POP:
        if (menuStack.size() > 1)
        {
            menuStack.pop_back();
        }
        break;
    case MenuAction::NONE:
        /* code */
        break;
    default:
        break;
    }
    
    //switch (event.type)
    //{
    //case psyqo::AdvancedPad::Event::ButtonPressed:
    //    switch (event.button)
    //    {
    //        case psyqo::AdvancedPad::Cross:
    //        {
    //            if (menuIndex == 0)
    //            {
    //                Gamepad::Instance().GetGamepad().setOnEvent(nullptr);
    //                AssetManager::Instance().filesToLoad = {"ARCHER.TIM;1", "TILEMAP.TIM;1", "MAP1.MAP;1", "SPRITES.TIM;1"};
    //                loadScene = new LoadingScreenScene();
    //                pushScene(loadScene);
    //            }
    //            else if (menuIndex == 1)
    //            {
//
    //            }
    //        }
    //        case psyqo::AdvancedPad::Triangle:
    //        {
//
    //        }
    //        break;
//
    //        case psyqo::AdvancedPad::Down:
    //            selectorPos.y += rowSeparateValue;
    //            if (++menuIndex > nMenuElements)
    //            {
    //                menuIndex = 0;
    //                selectorPos.y = firstRowPos.y;
    //            }
    //        break;
//
    //        case psyqo::AdvancedPad::Up:
    //            selectorPos.y -= rowSeparateValue;
    //            if (--menuIndex < 0)
    //            {
    //                menuIndex = nMenuElements;
    //                selectorPos.y = firstRowPos.y + nMenuElements * rowSeparateValue;
    //            }
    //        break;
    //    }
    //    break;
    //break;
    //case psyqo::AdvancedPad::Event::ButtonReleased:
    //    
    //    break;
    //case psyqo::AdvancedPad::Event::PadConnected:
    //    
    //    break;
    //case psyqo::AdvancedPad::Event::PadDisconnected:
    //    
    //    break;
    //
    //default:
    //    break;
    //}
    
}
