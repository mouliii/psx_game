#pragma once

#include "psyqo/advancedpad.hh"

class Gamepad 
{
public:
    Gamepad(const Gamepad&) = delete;
    Gamepad& operator=(const Gamepad&) = delete;

    static void Initialize(psyqo::AdvancedPad::PollingMode mode);
    static Gamepad& Instance() { return *m_instance; }
    psyqo::AdvancedPad& GetGamepad(){return m_gamepad;}
private:
    Gamepad() = default;
    ~Gamepad() = default;
    static Gamepad* m_instance;

    psyqo::AdvancedPad m_gamepad;
};