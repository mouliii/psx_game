#pragma once

#include "psyqo/advancedpad.hh"
#include "psyqo/fixed-point.hh"

class Gamepad 
{
public:
    Gamepad(const Gamepad&) = delete;
    Gamepad& operator=(const Gamepad&) = delete;

    static void Initialize(psyqo::AdvancedPad::PollingMode mode);
    static Gamepad& Instance() { return *m_instance; }
    psyqo::AdvancedPad& GetGamepad(){return m_gamepad;}
    psyqo::FixedPoint<> leftStickDeadZone = 0.2;
    psyqo::FixedPoint<> rightStickDeadZone = 0.2;
private:
    Gamepad() = default;
    ~Gamepad() = default;
    static Gamepad* m_instance;

    psyqo::AdvancedPad m_gamepad;
};