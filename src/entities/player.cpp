#include "player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Update(psyqo::AdvancedPad &pad)
{
    static const auto WHITE = psyqo::Color{{.r = 255, .g = 255, .b = 255}};
    static const auto GRAY = psyqo::Color{{.r = 48, .g = 48, .b = 48}};
    dir = {0,0};
    if (pad.isButtonPressed(psyqo::AdvancedPad::Pad::Pad1a, psyqo::AdvancedPad::Button::Cross)){
        color = GRAY;
    }else{
        color = psyqo::Color{128,128,128};
    }
    if (pad.isButtonPressed(psyqo::AdvancedPad::Pad::Pad1a, psyqo::AdvancedPad::Button::Up)){
        dir.y = -1;
    }
    if (pad.isButtonPressed(psyqo::AdvancedPad::Pad::Pad1a, psyqo::AdvancedPad::Button::Down)){
        dir.y = 1;
    }
    if (pad.isButtonPressed(psyqo::AdvancedPad::Pad::Pad1a, psyqo::AdvancedPad::Button::Left)){
        dir.x = -1;
    }
    if (pad.isButtonPressed(psyqo::AdvancedPad::Pad::Pad1a, psyqo::AdvancedPad::Button::Right)){
        dir.x = 1;
    }
    
    auto len = (dir.x * dir.x) + (dir.y * dir.y);
    if (len != 0 )
    {
        psyqo::Vec3 dirNormalized = psyqo::Vec3{dir.x, dir.y, 0};
        psyqo::SoftMath::normalizeVec3(&dirNormalized);
        pos += psyqo::Vec2{dirNormalized.x, dirNormalized.y} * stats.speed;
    }
    
    if (dir.x != 0){
        anim.ResumeAnimation();
        lastFacing = dir.x.integer();
    }else{
        anim.PauseAnimation();
    }
    anim.Update();
}

void Player::Draw(Graphics &gfx, int layer)
{
    psyqo::Rect rect = anim.GetFrame();
    if (lastFacing < 0)
    {
        anim.InvertX(rect);
    }
    gfx.DrawTexturedQuad(tex, pos, size, rect, layer, color);
    //gfx.DrawSprite16x16(tex, pos, rect, layer, color);
    //gfx.SetTpage(tex, layer);
}

void Player::Attack(Character *character)
{
    character->TakeDamage(stats.damage);
}

void Player::TakeDamage(int16_t amount)
{
    stats.health -= amount;
}
