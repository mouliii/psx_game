#pragma once

#include "game_objects/character.h"
#include "core_systems/gamepad.h"

class Player : public Character
{
public:
    Player();
    ~Player() override;
    void Update() override;
    void HandleDigitalPad();
    void HandleAnalogPad();
    void Draw(Graphics& gfx, int layer = 3) override;
    // TODO: projectile
    void Attack(Character* character) override;
    void TakeDamage(int16_t amount) override;
private:
    psyqo::Vec2 leftStick;
    psyqo::Vec2 rightStick;
};