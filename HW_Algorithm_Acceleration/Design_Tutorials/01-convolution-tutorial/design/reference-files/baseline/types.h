#pragma once

struct RGBPixel
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
}__attribute__((packed,aligned(4)));

typedef unsigned char GrayPixel;
