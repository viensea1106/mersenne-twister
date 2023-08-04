/* An implementation of the MT19937 Algorithm for the Mersenne Twister by Evan Sultanik.  
 * Based on the pseudocode from M. Matsumoto and T. Nishimura's paper:
 *      """ Mersenne Twister: A 623-dimensionally equidistributed uniform pseudorandom 
 *       number generator," ACM Transactions on Modeling and Computer Simulation 
 *       Vol. 8, No. 1, January pp.3-30 1998. """
 * Reference: http://www.sultanik.com/Mersenne_twister
 */

#include "mt19937.h"

/*
    Initialize vector `state[N]` from `seed` value.
*/
inline static void _mt19937_init_state(MT19937* rand, uint32_t seed) {
    rand->state[0] = seed & WORD_MASK;

    for (int i=1; i<N; ++i) 
        rand->state[i]  = F * (rand->state[i-1] ^ (rand->state[i-1] >> (W - 2))) + i;

    rand->index=N;
}

/*
    Twisting step: this step applies a linear transformation 
    to the `state` array to update the new internal `state`.
*/
inline static void _mt19937_twist(MT19937* rand) {
    for (int i=0; i<N; ++i) {
        uint32_t x  = (rand->state[i] & UPPER_MASK) | (rand->state[(i + 1)%N] & LOWER_MASK);
        uint32_t xA = x >> 1;

        if (x&1) xA ^= MATRIX_A;
        rand->state[i] = rand->state[(i + M)%N] ^ xA;
    }
    rand->index = 0;
}

/*
    Tempering step: this final step to generate output from
    current `state` values.
*/
inline static uint32_t _mt19937_tempering(MT19937* rand) {
    uint32_t y = rand->state[rand->index];
    y ^= (y >> U);
    y ^= (y << S) & MASK_B;
    y ^= (y << T) & MASK_C;
    y ^= (y >> L);

    rand->index++;
    return y; 
}

/*
    Create a new MT19937 generator object from give `seed`.
*/
MT19937 mt19937_seed(uint32_t seed) {
    MT19937 obj;
    _mt19937_init_state(&obj, seed);
    return obj;
}

/*
    Generate a 32-bits "random" number from MT19937 context.
*/
uint32_t mt19937_rand(MT19937* rand) {
    if (rand->index >= N)
        _mt19937_twist(rand);
    
    uint32_t output = _mt19937_tempering(rand);
    return output;
}


/*
    Recover `x` from `y` when:
        `y = x ^ ((x >> shift) & mask)` 
*/
inline static uint32_t _mt19937_untempering_right(uint32_t y, uint32_t shift, uint32_t mask) {
    uint32_t x=0;
    for (int i=shift; i<W; i+=shift) {
        uint32_t m = ((1 << i) - 1) << (W - i);
        x = (y ^ ((x >> shift) & mask)) & m;
    }
    x = (y ^ ((x >> shift) & mask)) & WORD_MASK;
    return x;
}

/*
    Recover `x` from `y` when:
        `y = x ^ ((x << shift) & mask)` 
*/
inline static uint32_t _mt19937_untempering_left(uint32_t y, uint32_t shift, uint32_t mask) {
    uint32_t x=0;
    for (int i=shift; i<W; i+=shift) {
        uint32_t m = (1 << i) - 1;
        x = (y ^ ((x << shift) & mask)) & m;
    }
    x = (y ^ ((x << shift) & mask)) & WORD_MASK;
    return x;
}

MT19937 mt19937_cracker(uint32_t outputs[N]) {
    MT19937 cracker;
    cracker.index=0;

    for (int *i=&cracker.index; *i<N; (*i)++) {
        uint32_t *x = &cracker.state[*i];
        uint32_t  y = outputs[*i];

        // untempering
        *x = _mt19937_untempering_right(y, L, WORD_MASK);
        *x = _mt19937_untempering_left(*x, T, MASK_C);
        *x = _mt19937_untempering_left(*x, S, MASK_B);
        *x = _mt19937_untempering_right(*x, U, WORD_MASK);
    }

    return cracker;
}