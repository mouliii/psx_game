#pragma once

#include "psyqo/primitives/common.hh"
#include "EASTL/vector.h"

// TODO: Rename to AnimationPlayer and have Animation struct.
// pAnim -> Update(Animation*);
// Animation CreateAnimation(uint16_t vramX, uint16_t vramY, uint16_t width, uint16_t height, uint16_t nFrames, uint16_t holdTimeInFrames);
// singleton?   

class Animation
{
public:
    Animation() = default;
    Animation(uint16_t vramX, uint16_t vramY, uint16_t width, uint16_t height, uint16_t nFrames, uint16_t holdTimeInFrames);
    void Update();
    void PauseAnimation();
    void ResumeAnimation();
    // NYI
    void ReverseAnimation();
    psyqo::Rect& GetFrame();
    void InvertX(psyqo::Rect& rect);
public:
    uint16_t framesElapsed = 0;
    uint16_t holdTimeInFrames;
    uint16_t curFrame = 0;
    uint16_t nFrames;
    uint16_t frameWidth;
    uint16_t frameHeight;
    bool paused = false;
    // eastl::fixed_string name
    eastl::vector<psyqo::Rect> frames;
};
