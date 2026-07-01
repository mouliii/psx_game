#include "enemy.h"

Enemy::Enemy()
{
    group = Group::ENEMIES;
    attackCooldown = 60;
    currentCooldown = 0;
}

Enemy::~Enemy()
{}

void Enemy::Update(const psyqo::Vec2& target)
{
    if (currentCooldown > 0)
    {
        currentCooldown--;
    }
    dir = {0,0};
    const psyqo::Vec2 deltaPos = target - pos;
    const int32_t distSq = deltaPos.x.integer() * deltaPos.x.integer() + deltaPos.y.integer() * deltaPos.y.integer();

    if (distSq > (threshold * threshold))
    {
        psyqo::Vec3 dirVecNorm{deltaPos.x, deltaPos.y, 0};
        psyqo::SoftMath::normalizeVec3(&dirVecNorm);
        dir = {dirVecNorm.x, dirVecNorm.y};
        pos += dir * stats.speed;
    }
    else
    {
        // melee attack
    }

    if (dir.x != 0){
        anim.ResumeAnimation();
        lastFacing = dir.x.integer();
    }else{
        anim.PauseAnimation();
    }

    anim.Update();
}

void Enemy::Draw(int layer)
{
    psyqo::Rect rect = anim.GetFrame();
    if (lastFacing < 0)
    {
        anim.InvertX(rect);
    }
    
    Graphics::Instance().DrawTexturedQuad(tex, pos, size, rect, layer, color);
}

void Enemy::Attack(Character* character)
{
    // if constexpr melee/ranged
    character->TakeDamage(stats.damage);
    currentCooldown = 60;
}

void Enemy::TakeDamage(int16_t amount)
{
    stats.health -= amount;
    if (stats.health <= 0)
    {
        alive = false;
        stats.health = 0;
    }
}
