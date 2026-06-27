#pragma once
#include "common/syscalls/syscalls.h"
#include "psyqo/application.hh"
#include "psyqo/font.hh"
#include "psyqo/gpu.hh"
#include "psyqo/scene.hh"
#include "psyqo/coroutine.hh"
#include "psyqo/primitives.hh"
#include "psyqo/task.hh"
#include "psyqo/fragments.hh"
//#include "psyqo/trigonometry.hh"
#include "core_systems/trig.h"

#include "core_systems/graphics.h"
#include "core_systems/asset_manager.h"
#include "core_systems/gamepad.h"
#include "game_systems/camera.h"
#include "game_systems/tilemap.h"
#include "entities/player.h"
#include "entities/enemy.h"
#include "core_systems/rng.h"

class GameScene: public psyqo::Scene
{
    void start(Scene::StartReason reason) override;
    void frame() override;
    void teardown(Scene::TearDownReason reason) override;
    void Draw();
    void Update();
    void SpawnNewEnemies();
public:
    Camera2D camera = Camera2D({Camera2D::CAMERA_OFFSET.x,Camera2D::CAMERA_OFFSET.y});
    Player player;
    eastl::vector<Enemy> enemies;
    Tilemap* tilemap;
    unsigned timer;
    uint32_t period;
    //psyqo::Trig<> trig;
};