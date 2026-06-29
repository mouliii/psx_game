#pragma once
#include "common/syscalls/syscalls.h"
#include "psyqo/application.hh"
#include "psyqo/font.hh"
#include "psyqo/gpu.hh"
#include "psyqo/scene.hh"
#include "EASTL/fixed_vector.h"
#include "EASTL/functional.h"

#include "core_systems/graphics.h"
#include "core_systems/asset_manager.h"
#include "core_systems/gamepad.h"
#include "game_systems/camera.h"
#include "scenes/game_scene.h"
#include "scenes/loading_screen.h"
#include "game_systems/menu_object.h"

class GameScene;
class LoadingScreenScene;

class MainMenuScene: public psyqo::Scene
{
public:
    MainMenuScene();
    void start(Scene::StartReason reason) override;
    void frame() override;
    void teardown(Scene::TearDownReason reason) override;
    void Draw();
    void Update();
    void StartGame();
    void ButtonEvents(const psyqo::AdvancedPad::Event& event);
public:
    Camera2D camera = Camera2D({Camera2D::CAMERA_OFFSET.x,Camera2D::CAMERA_OFFSET.y});
    psyqo::Vec2 selectorPos;
    psyqo::Vec2 firstRowPos;
    uint16_t rowSeparateValue = 16;
    uint16_t menuIndex;
    uint16_t nMenuElements;
    int16_t lastPressedButton = 0;
    GameScene* gameScene;
    LoadingScreenScene* loadScene;

    MenuObject mainMenu;
    MenuObject subMenu;
    eastl::vector<MenuObject*> menuStack;
};