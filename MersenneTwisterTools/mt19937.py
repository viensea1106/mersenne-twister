from .utils import MersenneTwister

class CONST:
    W = 32
    N = 624
    M = 397
    R = 31
    A = 0x9908b0df
    U = 11
    D = 0xffffffff
    S = 7
    B = 0x9d2c5680
    T = 15
    C = 0xefc60000
    L = 18
    F = 1812433253

class MT19937(MersenneTwister):
    def __init__(self, SEED) -> None:
        super().__init__(
            w = CONST.W, 
            n = CONST.N, 
            m = CONST.M, 
            r = CONST.R, 
            a = CONST.A, 
            u = CONST.U, 
            d = CONST.D, 
            s = CONST.S, 
            b = CONST.B, 
            t = CONST.T, 
            c = CONST.C, 
            l = CONST.L, 
            f = CONST.F
        )

        self._seed(SEED)