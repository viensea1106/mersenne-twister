#include <stdint.h>

#ifndef __MT19937_H
#define __MT19937_H

/*  Reference: https://cplusplus.com/reference/random/mt19937/
    Period parameters.
*/
#define N           624             
#define M           397               
#define R           31             
#define W           32
#define F           1812433253         
#define MATRIX_A    0x9908B0DF      
#define LOWER_MASK  0x7FFFFFFF     
#define UPPER_MASK  0x80000000  
#define WORD_MASK   0xFFFFFFFF

/*  Reference: https://cplusplus.com/reference/random/mt19937/
    Tempering parameters.
*/
#define MASK_B      0x9D2C5680     
#define MASK_C      0xEFC60000      
#define U           11              
#define L           18               
#define S           7                 
#define T           15

/*
    Mersenne Twister 19937 generator object.
*/ 
typedef struct MT19937 {
    uint32_t state[N];
    int index;    
} MT19937;

/*
    These are some core functions for implementation of MT19937:
    -   `mt19937_seed`      : return `MT19937` object which has initial `state` corresponding to `seed` value.
    -   `mt19937_rand`      : return "random" 32-bits number.
*/
MT19937  mt19937_seed(uint32_t seed);
uint32_t mt19937_rand(MT19937* rand);

/*
    Crack MT19937: Recovers the state from an MT19937 instance using 624(=N) outputs.
    NOTE: No twist should have been performed during the outputs.
    -   `mt19937_cracker`   : return a cloned MT19937 object.
*/
MT19937 mt19937_cracker(uint32_t outputs[N]);

#endif /* #ifndef __MT19937_H */