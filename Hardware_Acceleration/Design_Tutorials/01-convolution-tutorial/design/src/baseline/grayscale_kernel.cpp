
#include "constants.h"
#include "kernels.h"
#include "types.h"

extern "C"
{

void grayscale_cpu(const RGBPixel* inFrame, GrayPixel* outFrame, int img_height, int img_width)
{
    for (int pixel = 0; pixel < img_height * img_width; ++pixel)
    {
        GrayPixel gray = (inFrame[pixel].r * 0.30) + //red
                         (inFrame[pixel].g * 0.59) + // green
                         (inFrame[pixel].b * 0.11);  // blue
        outFrame[pixel] = gray;
    }
}

}
