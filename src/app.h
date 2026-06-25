#include "common/syscalls/syscalls.h"
#include "psyqo/application.hh"
#include "psyqo/font.hh"
#include "psyqo/gpu.hh"
#include "psyqo/scene.hh"

#include "scenes/main_menu.h"
#include "scenes/loading_screen.h"
#include "core_systems/cdrom_loader.h"
#include "core_systems/asset_manager.h"
#include "core_systems/graphics.h"

class RootScene: public psyqo::Scene
{
    void start(Scene::StartReason reason) override
    {
        switch (reason)
        {
        case StartReason::Create:
        {
            syscall_puts("initilaizing systems...\n");
            CdRomLoader::Initialize();
            AssetManager::Initialize();
            Graphics::Initialize(gpu());
            syscall_puts("done.\n");
            // load font
            AssetManager::Instance().filesToLoad = {"FONT.TIM;1"};
            m_coro = AssetManager::Instance().LoadLevel(AssetManager::Instance().filesToLoad, gpu());
            m_coro.resume();
        break;
        }
        case StartReason::Resume:
        {
            delete loadingScreen;
            mainMenu = new MainMenuScene();
            pushScene(mainMenu);
        break;
        }
        default:
            break;
        }
    }

    void frame() override
    {
        if (m_coro.done())
        {
            Graphics::Instance().SetActiveFont(AssetManager::Instance().GetTexture("FONT.TIM;1", gpu()));
            loadingScreen = new LoadingScreenScene();
            AssetManager::Instance().filesToLoad = {"AWESOME.TIM;1", "BGLEFT.TIM;1", "BGRIGHT.TIM;1", "SPRITES.TIM;1"};
            pushScene(loadingScreen);
        }
        
    }

    void teardown(Scene::TearDownReason reason) override
    {
        if (reason == TearDownReason::Destroy)
        {
            delete mainMenu;
        }
    }
private:
    MainMenuScene* mainMenu;
    LoadingScreenScene* loadingScreen;
    psyqo::Coroutine<> m_coro;
};

class App : public psyqo::Application
{
    void prepare() override
    {
        psyqo::GPU::Configuration config;
        config.set(psyqo::GPU::Resolution::W320)
        .set(psyqo::GPU::VideoMode::AUTO)
        .set(psyqo::GPU::ColorMode::C15BITS)
        .set(psyqo::GPU::Interlace::PROGRESSIVE);
        gpu().initialize(config);
    }
    void createScene() override
    {
        pushScene(&rootScene);
    }
private:
    RootScene rootScene;
};

/*
class RootScene: public psyqo::Scene
{
    void start(Scene::StartReason reason) override
    {
        m_systemFont.uploadSystemFont(gpu());
        m_romFont.uploadKromFont(gpu(), {{.x = 960, .y = int16_t(512 - 48 - 90)}});
    }

    void frame() override
    {
        auto frameCount = gpu().getFrameCount();
        counter += 4;
        psyqo::FixedPoint pos_y = 1;
        psyqo::FixedPoint<10> asd;
        asd.value = counter;
        auto sin = trig.sin(psyqo::Angle(asd));
        pos_y *= sin;
        sin += 1;
        sin /= 2;
        pos_y = sin * 150;
        psyqo::Color c = {{.r = 0, .g = 0, .b = (sin * 255).integer()}};
        gpu().clear(c);
        eastl::basic_string<char> str;
        str.append_sprintf("%i", sin.value); // pos_y.integer

        psyqo::Color texColor = {{.r = 255, .g = 255, .b = uint8_t(255 - static_cast<uint8_t>(frameCount))}};
        m_systemFont.print(gpu(), "Hello World!", {{.x = 16, .y = 32}}, {255,255,255});
        m_romFont.print(gpu(), str, {{.x = 16, .y = pos_y.integer() + 20}}, texColor);
        psyqo::Color quadColor = {{.r = 255, .g = 100, .b = 3}};
        psyqo::Prim::Rectangle8x8 sprite(quadColor);
        //sprite.position = {.x = 150, .y = 100};
        sprite.position = {150, 100};
        gpu().sendPrimitive(sprite);
    }

    void teardown(Scene::TearDownReason reason) override
    {}
public:
    psyqo::Trig<12> trig;
    psyqo::Font<> m_systemFont;
    psyqo::Font<> m_romFont;
    uint32_t counter;
};
*/