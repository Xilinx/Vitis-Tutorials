
#include "constants.h"
#include "kernels.h"
#include "types.h"

#include <cmath>

extern "C"
{

void convolve_cpu(const RGBPixel* inFrame, RGBPixel* outFrame,
                  const float* coefficient, int coefficient_size,
                  int img_width, int img_height)
{
    int center = coefficient_size / 2;
    for(int line = 0; line < img_height; ++line)
    {
        for(int pixel = 0; pixel < img_width; ++pixel)
        {
            float sum_r = 0, sum_g = 0, sum_b = 0;
            for(int m = 0; m < coefficient_size; ++m)
            {
                for(int n = 0; n < coefficient_size; ++n)
                {
                    int ii = line + m - center;
                    int jj = pixel + n - center;

                    if(ii >= 0 && ii < img_height && jj >= 0 && jj < img_width)
                    {
                        sum_r += inFrame[(ii * img_width) + jj].r * coefficient[(m * coefficient_size) + n];
                        sum_g += inFrame[(ii * img_width) + jj].g * coefficient[(m * coefficient_size) + n];
                        sum_b += inFrame[(ii * img_width) + jj].b * coefficient[(m * coefficient_size) + n];
                    }
                }
            }
            outFrame[line * img_width + pixel].r = fabsf(sum_r);
            outFrame[line * img_width + pixel].g = fabsf(sum_g);
            outFrame[line * img_width + pixel].b = fabsf(sum_b);
        }
    }
}

}
