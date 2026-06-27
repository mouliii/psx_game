#include "gamepad.h"

Gamepad* Gamepad::m_instance = nullptr;

void Gamepad::Initialize(psyqo::AdvancedPad::PollingMode mode)
{
    if (m_instance == nullptr) 
    {
        m_instance = new Gamepad();
    }
    Gamepad::Instance().m_gamepad.initialize(mode);
}