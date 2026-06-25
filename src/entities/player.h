#pragma once

#include "entities/character.h"

class Player : public Character
{
public:
    Player();
    ~Player() override;
    void Update(psyqo::AdvancedPad& pad) override;
    void Draw(Graphics& gfx, int layer = 3) override;
    // TODO: projectile
    void Attack(Character* character) override;
    void TakeDamage(int16_t amount) override;
private:

};