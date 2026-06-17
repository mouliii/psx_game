#pragma once

#include "common/syscalls/syscalls.h"
#include "psyqo/gpu.hh"
#include "psyqo/font.hh"
#include "psyqo/scene.hh"
#include "psyqo/coroutine.hh"
#include "psyqo/task.hh"

#include "core_systems/graphics.h"
#include "core_systems/asset_manager.h"

class LoadingScreenScene: public psyqo::Scene
{
    void start(Scene::StartReason reason) override;
    void frame() override;
    void teardown(Scene::TearDownReason reason) override;

public:
    psyqo::Coroutine<> m_loaderCoro;
    psyqo::Coroutine<Texture*> m_texCoro;
    psyqo::Font<> m_systemFont;
};