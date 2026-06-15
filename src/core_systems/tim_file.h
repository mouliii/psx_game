#pragma once

#include <stdint.h>
#include "psyqo/xprintf.h"


struct TIM_File
{
    struct Header
    {
        uint8_t id; // must be 0x10
        uint8_t version; // should be 0x00
        uint16_t reserved = 0;
        uint8_t colorMode; // 0-3 bits
        bool clutExistsFlag;
    };
    struct CLUT
    {
        uint32_t clutSize; // actual data without header
        uint16_t clutX; // must be even multiple of 16
        uint16_t clutY;
        uint16_t clutWidth; // 4bit - 16, 8bit - 256 pixels
        uint16_t clutHeight;
        uint16_t* pClutData;
    };
    struct ImageData
    {
        uint32_t imgSize; // actual data without header
        uint16_t imgX;
        uint16_t imgY;
        uint16_t imgWidth;
        uint16_t imgHeight;
        uint16_t* pImgData;
    };
    Header header;
    CLUT clut;
    ImageData image;
};

struct TextureInfo
{
    uint8_t colorMode;
    bool clutExistsFlag;
    uint16_t clutX;
    uint16_t clutY;
    uint16_t clutWidth;
    uint16_t clutHeight;
    uint16_t imgX;
    uint16_t imgY;
    uint16_t imgWidth;
    uint16_t imgHeight;
};