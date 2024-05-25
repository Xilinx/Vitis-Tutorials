/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include "kernels.h"
#include <stdio.h>

// template <typename T, int R, int C>
// void printRegister(char *MatName, aie::vector<T, R * C> r)
// {
//     for (int i = 0; i < R; i++)
//     {
//         for (int j = 0; j < C; j++)
//             printf("%10d ", (int)r[j + i * C]);
//         printf("\n");
//     }
//     printf("\n");
// }
//
// #define DISPLAY(a) printf(#a ": %d", a)
// #define PrintMatrices                                            \
//     printRegister<ITYPE, ATILES_ROWS, ATILES_COLS>("A", a); \
//     printRegister<ITYPE, BTILES_ROWS, BTILES_COLS>("B", b); \
//     printRegister<OTYPE, CTILES_ROWS, CTILES_COLS>("C", ctile.to_vector<OTYPE>(SHIFT_RESULT));
// #define PrintMatrices_1                                            \
//     printRegister<ITYPE, ATILES_ROWS, ATILES_COLS>("A1", a1); \
//     printRegister<ITYPE, ATILES_ROWS, ATILES_COLS>("A2", a2); \
//     printRegister<ITYPE, BTILES_ROWS, BTILES_COLS>("B", b); \
//     printRegister<OTYPE, CTILES_ROWS, CTILES_COLS>("C1", ctile1.to_vector<OTYPE>(SHIFT_RESULT));\
//     printRegister<OTYPE, CTILES_ROWS, CTILES_COLS>("C2", ctile2.to_vector<OTYPE>(SHIFT_RESULT));


#if OPTIMIZED_SOURCE==0
template<typename ITYPE,typename OTYPE, int SHIFT_RESULT>
void ClassicMatMult(adf::input_buffer<ITYPE,adf::extents<adf::inherited_extent,adf::inherited_extent>> & __restrict inA,
    adf::input_buffer<ITYPE,adf::extents<adf::inherited_extent,adf::inherited_extent>> & __restrict inB,
    adf::output_buffer<OTYPE,adf::extents<adf::inherited_extent,adf::inherited_extent>> & __restrict outC)
    {
        constexpr size_t sizeTileA = ATILES_ROWS * ATILES_COLS;
        constexpr size_t sizeTileB = BTILES_ROWS * BTILES_COLS;
        constexpr size_t sizeTileC = CTILES_ROWS * CTILES_COLS;

        constexpr size_t NTilesPerRow_A = A_ROWS / ATILES_ROWS;
        constexpr size_t NTilesPerCol_A = A_COLS / ATILES_COLS;
        constexpr size_t NTilesPerRow_B = B_ROWS / BTILES_ROWS;
        constexpr size_t NTilesPerCol_B = B_COLS / BTILES_COLS;
        constexpr size_t NTilesPerRow_C = C_ROWS / CTILES_ROWS;
        constexpr size_t NTilesPerCol_C = C_COLS / CTILES_COLS;

        auto  pA = aie::begin_vector<sizeTileA>(inA);
        auto  pB = aie::begin_vector<sizeTileB>(inB);
        auto  pC = aie::begin_vector<sizeTileC>(outC);

        aie::mmul<ATILES_ROWS, ATILES_COLS, CTILES_COLS, ITYPE, ITYPE, acc32> ctile;

        for (int i = 0; i < NTilesPerRow_C; i++)
        {
            for (int j = 0; j < NTilesPerCol_C; j++)
            chess_prepare_for_pipelining
            {
                auto a = *pA++;
                auto b = *pB++;
                ctile.mul(a, b);

                for (int k = 1; k < NTilesPerCol_A; k++)
                // chess_unroll_loop(*)
                chess_flatten_loop
                {
                    a = *pA++;
                    b = *pB++;
                    ctile.mac(a, b);
                }

                *pC++ = ctile.template to_vector<OTYPE>(SHIFT_RESULT);

                pA -= NTilesPerCol_A; // Back to begining of row
                // For matrix B the next tile is used
            }
            pA += NTilesPerCol_A;                  // Next Row
            pB -= NTilesPerCol_B * NTilesPerRow_B; // Back to begining of matrix B
        }
    }

#elif OPTIMIZED_SOURCE == 1
template<typename ITYPE,typename OTYPE, int SHIFT_RESULT>
void ClassicMatMult(adf::input_buffer<ITYPE, adf::extents<adf::inherited_extent, adf::inherited_extent>> &__restrict inA,
adf::input_buffer<ITYPE, adf::extents<adf::inherited_extent, adf::inherited_extent>> &__restrict inB,
adf::output_buffer<OTYPE, adf::extents<adf::inherited_extent, adf::inherited_extent>> &__restrict outC)
{
    constexpr size_t sizeTileA = ATILES_ROWS * ATILES_COLS;
    constexpr size_t sizeTileB = BTILES_ROWS * BTILES_COLS;
    constexpr size_t sizeTileC = CTILES_ROWS * CTILES_COLS;

    constexpr size_t NTilesPerRow_A = A_ROWS / ATILES_ROWS;
    constexpr size_t NTilesPerCol_A = A_COLS / ATILES_COLS;
    constexpr size_t NTilesPerRow_B = B_ROWS / BTILES_ROWS;
    constexpr size_t NTilesPerCol_B = B_COLS / BTILES_COLS;
    constexpr size_t NTilesPerRow_C = C_ROWS / CTILES_ROWS;
    constexpr size_t NTilesPerCol_C = C_COLS / CTILES_COLS;

    auto pA = aie::begin_vector<sizeTileA>(inA);
    auto pB = aie::begin_vector<sizeTileB>(inB);
    auto pC = aie::begin_vector<sizeTileC>(outC);

    aie::mmul<ATILES_ROWS, ATILES_COLS, CTILES_COLS, ITYPE, ITYPE, acc32> ctile1;
    aie::mmul<ATILES_ROWS, ATILES_COLS, CTILES_COLS, ITYPE, ITYPE, acc32> ctile2;

    for (int i = 0; i < NTilesPerRow_C / 2; i++)
    {
        for (int j = 0; j < NTilesPerCol_C; j++)
        chess_prepare_for_pipelining
        chess_loop_range(4, )
        {
            auto a1 = *pA++;
            auto a2 = *pA++;
            auto b = *pB++;
            ctile1.mul(a1, b);
            ctile2.mul(a2, b);

            for (int k = 1; k < NTilesPerCol_A; k++)
            chess_flatten_loop
            {
                a1 = *pA++;
                a2 = *pA++;
                b = *pB++;
                ctile1.mac(a1, b);
                ctile2.mac(a2, b);
            }

            *pC++ = ctile1.template to_vector<OTYPE>(SHIFT_RESULT);
            *pC++ = ctile2.template to_vector<OTYPE>(SHIFT_RESULT);

            pA -= 2 * NTilesPerCol_A; // Back to begining of row
            // For matrix B the next tile is used
        }
        pA += 2 * NTilesPerCol_A;              // Next Row
        pB -= NTilesPerCol_B * NTilesPerRow_B; // Back to begining of matrix B
    }
}

#else
#error "OPTIMIZED_SOURCE should be defined as 0 or 1"
#endif
