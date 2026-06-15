#include "player.h"

Player::Player(Texture *tex, psyqo::Vec2 pos, psyqo::Vertex size)
        :
        tex(tex), pos(pos), size(size)
    {
        psyqo::Vertex uv = Graphics::Instance().GetTextureUV(tex);
        anim = Animation(uv.x, uv.y, 16, 16, 4, 10);
    }
Player::~Player()
{}

void Player::Update(psyqo::AdvancedPad &pad)
{
    static const auto WHITE = psyqo::Color{{.r = 255, .g = 255, .b = 255}};
    static const auto GRAY = psyqo::Color{{.r = 48, .g = 48, .b = 48}};
    bool isMoving = false;
    if (pad.isButtonPressed(psyqo::AdvancedPad::Pad::Pad1a, psyqo::AdvancedPad::Button::Cross)){
        color = GRAY;
    }else{
        color = psyqo::Color{128,128,128};
    }
    if (pad.isButtonPressed(psyqo::AdvancedPad::Pad::Pad1a, psyqo::AdvancedPad::Button::Up)){
        pos.y -= speed;
        isMoving = true;
    }
    if (pad.isButtonPressed(psyqo::AdvancedPad::Pad::Pad1a, psyqo::AdvancedPad::Button::Down)){
        pos.y += speed;
        isMoving = true;
    }
    if (pad.isButtonPressed(psyqo::AdvancedPad::Pad::Pad1a, psyqo::AdvancedPad::Button::Left)){
        pos.x -= speed;
        dir = -1;
        isMoving = true;
    }
    if (pad.isButtonPressed(psyqo::AdvancedPad::Pad::Pad1a, psyqo::AdvancedPad::Button::Right)){
        pos.x += speed;
        dir = 1;
        isMoving = true;
    }
    if (!isMoving){
        anim.PauseAnimation();
    }else{
        anim.ResumeAnimation();
    }
    anim.Update();
}

void Player::Draw(Graphics &gfx, int layer)
{
    bool invert = dir < 0;
    psyqo::Rect rect = anim.GetFrame();
    if (invert)
    {
        anim.InvertX(rect);
    }
    
    gfx.DrawTexturedQuad(tex, pos, size, rect, layer, color);
}
