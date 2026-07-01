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
#include "game_objects/player.h"
#include "game_objects/enemy.h"
#include "game_objects/projectile.h"
#include "core_systems/rng.h"

class GameScene: public psyqo::Scene
{
public:
    void start(Scene::StartReason reason) override;
    void frame() override;
    void teardown(Scene::TearDownReason reason) override;
    void Draw();
    void Update();
    void InputEvents(const psyqo::AdvancedPad::Event& event);
    void SpawnNewEnemies();
    void SpawnProjectile(const Projectile projectile);
private:
    Camera2D camera = Camera2D({Camera2D::CAMERA_OFFSET.x,Camera2D::CAMERA_OFFSET.y});
    Player player;
    eastl::vector<Enemy> enemies;
    eastl::vector<Projectile> projectiles;
    Tilemap* tilemap;
    unsigned timer;
    uint32_t period;
};