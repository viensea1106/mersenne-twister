from .utils import MersenneTwister

class CONST:
    W = 64
    N = 312
    M = 156
    R = 31
    A = 0xb5026f5aa96619e9
    U = 29
    D = 0x5555555555555555
    S = 17
    B = 0x71d67fffeda60000
    T = 37
    C = 0xfff7eee000000000
    L = 43
    F = 6364136223846793005

class MT19937_64(MersenneTwister):
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