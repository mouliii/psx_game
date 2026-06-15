#include "animation.h"

Animation::Animation(uint16_t vramX, uint16_t vramY, uint16_t width, uint16_t height, uint16_t nFrames, uint16_t holdTimeInFrames)
    :
    holdTimeInFrames(holdTimeInFrames), nFrames(nFrames), frameWidth(width), frameHeight(height)
{
    frames.reserve(nFrames);
    for (uint16_t i = 0; i < nFrames; i++)
    {
        frames.emplace_back(psyqo::Rect{{static_cast<int16_t>(vramX + (i * width)), static_cast<int16_t>(vramY)}, {static_cast<int16_t>(width), static_cast<int16_t>(height)}});
    }
}

void Animation::Update()
{
    if (paused)
    {
        return;
    }
    
    framesElapsed++;
    if (framesElapsed >= holdTimeInFrames)
    {
        framesElapsed = 0;
        curFrame++;
        if(curFrame >= nFrames)
        {
            curFrame = 0;
        }
    }
}

void Animation::ReverseAnimation()
{
}

void Animation::InvertX(psyqo::Rect &rect)
{
    rect.pos.x += frameWidth;
    rect.size.x = -frameWidth;
}
