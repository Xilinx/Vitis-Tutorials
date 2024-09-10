// polyvec.h

#ifndef POLYVEC_H
#define POLYVEC_H

#include <stdio.h>
#include <iostream> // for std::cout

#include <stdint.h>

#define N 256 // Replace 'N' with the appropriate value
#define K 128   // Replace 'K' with the appropriate value 

#define Q 3329 // Replace 'Q' with the appropriate value
#define QINV -3327 // q^-1 mod 2^16

typedef struct {
    uint16_t coeff[N];
} poly;

typedef struct {
    poly vec[K];
} polyvec;

void polyvec_ntt(polyvec *vin,polyvec *vout);
void poly_ntt(poly *ain,poly *aout);
void ntt(uint16_t p_in[N], uint16_t p_out[N]);
//uint16_t montgomery_reduce(int32_t a);

#endif // POLYVEC_H
