#pragma once
#include "common/syscalls/syscalls.h"
#include "psyqo/application.hh"
#include "psyqo/font.hh"
#include "psyqo/gpu.hh"
#include "psyqo/scene.hh"
#include "psyqo/advancedpad.hh"

#include "core_systems/graphics.h"
#include "core_systems/asset_manager.h"
#include "game_systems/camera.h"
#include "scenes/game_scene.h"

class MainMenuScene: public psyqo::Scene
{
    void start(Scene::StartReason reason) override;
    void frame() override;
    void teardown(Scene::TearDownReason reason) override;
    void Draw();
    void Update();
public:
    Camera2D camera = Camera2D({Camera2D::CAMERA_OFFSET.x,Camera2D::CAMERA_OFFSET.y});
    psyqo::Font<32> m_systemFont;
    psyqo::Prim::TexturedQuad bgLeft;
    psyqo::Prim::TexturedQuad bgRight;
    psyqo::AdvancedPad m_gamePad;
    psyqo::Scene* gameScene;
};