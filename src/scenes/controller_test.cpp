#include "controller_test.h"

void ControllerTestScene::start(Scene::StartReason reason)
{
    m_gamePad.initialize(psyqo::AdvancedPad::PollingMode::Normal);
    m_gamePad.setOnEvent([this](psyqo::AdvancedPad::Event event)
    {
        ButtonEvents(event);
    });
}

void ControllerTestScene::frame()
{
}

void ControllerTestScene::teardown(Scene::TearDownReason reason)
{
}

void ControllerTestScene::Draw()
{
}

void ControllerTestScene::Update()
{
}

void ControllerTestScene::ButtonEvents(const psyqo::AdvancedPad::Event &event)
{
    using pad_type = psyqo::AdvancedPad::PadType;
    switch (m_gamePad.getPadType(psyqo::AdvancedPad::Pad::Pad1a))
    {
        case pad_type::AnalogPad:
            // draw digital pad
            // draw analog sticks
        // no break;
        case pad_type::DigitalPad:
            // drawi digital pad
        break;
        case pad_type::Mouse:
            // ???
        break;
    }

    switch (event.type)
    {
    case psyqo::AdvancedPad::Event::ButtonPressed:
        switch (event.button)
        {
            case psyqo::AdvancedPad::Cross:
            break;

            case psyqo::AdvancedPad::Down:
            break;

            case psyqo::AdvancedPad::Up:
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
