#include "player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Update()
{
    const auto pad = Gamepad::Instance().GetGamepad();
    static const auto WHITE = psyqo::Color{{.r = 255, .g = 255, .b = 255}};
    static const auto GRAY = psyqo::Color{{.r = 48, .g = 48, .b = 48}};
    dir = {0,0};
    if (pad.isButtonPressed(psyqo::AdvancedPad::Pad::Pad1a, psyqo::AdvancedPad::Button::Cross)){
        color = GRAY;
    }else{
        color = psyqo::Color{128,128,128};
    }
    
    if (pad.getPadType(psyqo::AdvancedPad::Pad::Pad1a) == pad.DigitalPad)
    {
        HandleDigitalPad();
    }
    else if (pad.getPadType(psyqo::AdvancedPad::Pad::Pad1a) == pad.AnalogPad)
    {
        HandleAnalogPad();
    }
    
    pos += psyqo::Vec2{dir.x, dir.y} * stats.speed;

    if (dir.x != 0){
        anim.ResumeAnimation();
        lastFacing = dir.x.integer();
    }else{
        anim.PauseAnimation();
    }
    anim.Update();
}

void Player::HandleDigitalPad()
{
    const auto pad = Gamepad::Instance().GetGamepad();
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
        dir = {dirNormalized.x, dirNormalized.y};
    }
}

void Player::HandleAnalogPad()
{
    const auto pad = Gamepad::Instance().GetGamepad();
    psyqo::FixedPoint axisRX{(int32_t)pad.getAdc(psyqo::AdvancedPad::Pad::Pad1a, 0), int32_t{0}};
    psyqo::FixedPoint axisRY{(int32_t)pad.getAdc(psyqo::AdvancedPad::Pad::Pad1a, 1), int32_t{0}};
    psyqo::FixedPoint axisLX{(int32_t)pad.getAdc(psyqo::AdvancedPad::Pad::Pad1a, 2), int32_t{0}};
    psyqo::FixedPoint axisLY{(int32_t)pad.getAdc(psyqo::AdvancedPad::Pad::Pad1a, 3), int32_t{0}};
    rightStick.x = (axisRX  / 255) * 2 - 1;
    rightStick.y = (axisRY  / 255) * 2 - 1;
    leftStick.x =  (axisLX  / 255) * 2 - 1;
    leftStick.y =  (axisLY  / 255) * 2 - 1;
    //printf("LX: %d, LY: %d\n", leftStick.x.raw(), leftStick.y.raw());

    auto len = leftStick.x * leftStick.x + leftStick.y * leftStick.y;
    //printf("Len %d\n", len.raw());

    if(len > (Gamepad::Instance().leftStickDeadZone * Gamepad::Instance().leftStickDeadZone))
    {
        psyqo::Vec3 dirNormalized = psyqo::Vec3{leftStick.x, leftStick.y, 0};
        psyqo::SoftMath::normalizeVec3(&dirNormalized);
        dir = {dirNormalized.x, dirNormalized.y};
    }
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
