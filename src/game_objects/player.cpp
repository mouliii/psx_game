#include "player.h"
#include "scenes/game_scene.h"

Player::Player()
{
    reticle.size = {16,16};
    reticle.texture = AssetManager::Instance().GetTexture("SPRITES.TIM;1", Graphics::Instance().GPU());
    reticle.uv = {{10*16, 0}, {16,16}};
    group = Group::ALLIES;
    attackCooldown = 60;
    currentCooldown = 0;
}

Player::~Player()
{
}

void Player::Update()
{
    if (currentCooldown > 0)
    {
        currentCooldown--;
    }
    
    const auto pad = Gamepad::Instance().GetGamepad();
    static const auto WHITE = psyqo::Color{{.r = 255, .g = 255, .b = 255}};
    static const auto GRAY = psyqo::Color{{.r = 48, .g = 48, .b = 48}};
    dir = {0,0};
    aimDir = {0,0};
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
    if (shooting)
    {
        if (currentCooldown <= 0){
            ShootProjectile();
        }
    }

    if (dir.x != 0){
        anim.ResumeAnimation();
        lastFacing = dir.x.integer();
    }else{
        anim.PauseAnimation();
    }
    anim.Update();
}

void Player::Draw(int layer)
{

    psyqo::Rect rect = anim.GetFrame();
    if (lastFacing < 0)
    {
        anim.InvertX(rect);
    }
    Graphics::Instance().DrawTexturedQuad(tex, pos, size, rect, layer, color);
    if (shooting)
    {
        reticle.position = {pos.x, pos.y};
        psyqo::Vec2 dp = {aimDir.x * 35, aimDir.y * 35};
        reticle.position += dp;
        //reticle.angle = 
        Graphics::Instance().DrawTexturedQuad(reticle, layer);
        Graphics::Instance().DrawCircle({pos.x, pos.y}, 6, 5);
    }
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
    psyqo::Vec2 leftStick;
    psyqo::Vec2 rightStick;
    rightStick.x = (axisRX  / 255) * 2 - 1;
    rightStick.y = (axisRY  / 255) * 2 - 1;
    leftStick.x =  (axisLX  / 255) * 2 - 1;
    leftStick.y =  (axisLY  / 255) * 2 - 1;
    //printf("LX: %d, LY: %d\n", leftStick.x.raw(), leftStick.y.raw());

    /* LEFT STICK - MOVEMENT */
    auto len = leftStick.x * leftStick.x + leftStick.y * leftStick.y;
    //printf("Len %d\n", len.raw());
    if(len > (Gamepad::Instance().leftStickDeadZone * Gamepad::Instance().leftStickDeadZone))
    {
        psyqo::Vec3 dirNormalized = psyqo::Vec3{leftStick.x, leftStick.y, 0};
        psyqo::SoftMath::normalizeVec3(&dirNormalized);
        dir = {dirNormalized.x, dirNormalized.y};
    }
    /* RIGHT STICK - ATTACK */
    len = rightStick.x * rightStick.x + rightStick.y * rightStick.y;
    if(len > (Gamepad::Instance().rightStickDeadZone * Gamepad::Instance().rightStickDeadZone))
    {
        psyqo::Vec3 dirNormalized = psyqo::Vec3{rightStick.x, rightStick.y, 0};
        psyqo::SoftMath::normalizeVec3(&dirNormalized);
        aimDir = {dirNormalized.x, dirNormalized.y};
        shooting = true;
    }
    else{
        shooting = false;
    }


}

// player takes ownership of projectile
void Player::SetupProjectile(Projectile projectile)
{
    this->projectile = projectile;
    projectile.stats.speed = 2;
    projectile.stats.damage = 15;
    projectile.group = ALLIES;
}

void Player::Attack(Character *character)
{
    //character->TakeDamage(stats.damage);
    //currentCooldown = 60;
}

void Player::ShootProjectile()
{
    projectile.pos = pos;
    projectile.dir = aimDir;
    scene->SpawnProjectile(projectile);
    currentCooldown = 60;
}

void Player::TakeDamage(int16_t amount)
{
    stats.health -= amount;
    if (stats.health <= 0)
    {
        alive = false;
        stats.health = 0;
    }
}
