#pragma once
#include "common/syscalls/syscalls.h"
#include "psyqo/application.hh"
#include "psyqo/font.hh"
#include "psyqo/gpu.hh"
#include "psyqo/scene.hh"

#include "core_systems/graphics.h"
#include "core_systems/asset_manager.h"
#include "core_systems/gamepad.h"
#include "game_systems/camera.h"
#include "scenes/game_scene.h"
#include "scenes/loading_screen.h"

class GameScene;
class LoadingScreenScene;

class MainMenuScene: public psyqo::Scene
{
    void start(Scene::StartReason reason) override;
    void frame() override;
    void teardown(Scene::TearDownReason reason) override;
    void Draw();
    void Update();
    void ButtonEvents(const psyqo::AdvancedPad::Event& event);
public:
    Camera2D camera = Camera2D({Camera2D::CAMERA_OFFSET.x,Camera2D::CAMERA_OFFSET.y});
    psyqo::Vec2 selectorPos{115,120};
    psyqo::Vec2 firstRowPos{115,120};
    int16_t menuIndex = 0;
    int16_t lastPressedButton = 0;
    GameScene* gameScene;
    LoadingScreenScene* loadScene;
};