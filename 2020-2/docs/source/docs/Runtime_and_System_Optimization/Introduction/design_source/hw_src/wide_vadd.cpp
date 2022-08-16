#include <hls_vector.h>

using vint16 = hls::vector<int, 16>;

void wide_vadd(vint16 *a, vint16 *b, vint16 *c, int size)
{
#pragma HLS INTERFACE m_axi bundle=in_a port=a depth=1024
#pragma HLS INTERFACE m_axi bundle=in_b port=b depth=1024
#pragma HLS INTERFACE m_axi bundle=out_c port=c depth=1024

    for (int i = 0; i < (size/16); i++)
    {
        c[i] = a[i] + b[i];
    }
}
