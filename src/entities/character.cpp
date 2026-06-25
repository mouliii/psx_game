#include "character.h"

Character::~Character()
{}

void Character::Initialize(Texture *tex, const psyqo::Vec2 pos, const psyqo::Vertex size, const Animation anim, const Stats &stats)
{
    this->tex = tex;
    this->pos = pos;
    this->size = size;
    this->anim = anim;
    this->stats = stats;
}

void Character::Update(psyqo::AdvancedPad &pad)
{
}

void Character::Draw(Graphics &gfx, int layer)
{
}

void Character::Attack(Character* character)
{
    character->TakeDamage(stats.damage);
}

void Character::TakeDamage(int16_t amount)
{
    stats.health -= amount;
}
