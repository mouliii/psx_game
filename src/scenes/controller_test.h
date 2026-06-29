#pragma once

#include "scenes/main_menu.h"

class ControllerTestScene: public psyqo::Scene
{
    void start(Scene::StartReason reason) override;
    void frame() override;
    void teardown(Scene::TearDownReason reason) override;
    void Draw();
    void Update();
    void ButtonEvents(const psyqo::AdvancedPad::Event& event);
public:
    Camera2D camera = Camera2D({Camera2D::CAMERA_OFFSET.x,Camera2D::CAMERA_OFFSET.y});
    psyqo::AdvancedPad m_gamePad;
};