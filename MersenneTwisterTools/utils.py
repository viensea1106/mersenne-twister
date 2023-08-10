class MersenneTwister:
    def __init__(self, w, n, m, r, a, u, d, s, b, t, c, l, f) -> None:
        self.W = w      # word size
        self.N = n      # state size
        self.M = m      # shift size
        self.R = r      # mask bits
        self.A = a      # xor mask
        self.U = u      # tempering u
        self.D = d      # tempering D
        self.S = s      # tempering S
        self.B = b      # tempering B
        self.T = t      # tempering T
        self.C = c      # tempering C
        self.L = l      # tempering L
        self.F = f      # initialization multiplier
        
        self.WORD_MASK  = (1 << self.W) - 1
        self.LOWER_MASK = (1 << self.R) - 1
        self.UPPER_MASK = (~self.LOWER_MASK) & self.WORD_MASK

        self.state = [0] * self.N
        self.index = self.N
    
    """
        Initialize vector `state[N]` from `seed` value.
    """
    def _seed(self, SEED):
        self.state[0] = SEED & self.WORD_MASK
        for i in range(1, self.N):
            self.state[i]  = self.F * (self.state[i-1] ^ (self.state[i-1] >> (self.W - 2))) + i
            self.state[i] &= self.WORD_MASK

    """
        Twisting step: this step applies a linear transformation 
        to the `state` array to update the new internal `state`.
    """
    def _twist(self):
        for i in range(self.N):
            x  = (self.state[i] & self.UPPER_MASK) | (self.state[(i + 1)%self.N] & self.LOWER_MASK)
            xA = x >> 1

            if (x&1): xA ^= self.A
            
            self.state[i] = self.state[(i + self.M)%self.N] ^ xA
        
        self.index = 0

    """
        Tempering step: this final step to generate output from
        current `state` values.
    """
    def _tempering(self):
        y = self.state[self.index]

        y ^= (y >> self.U) & self.D
        y ^= (y << self.S) & self.B
        y ^= (y << self.T) & self.C
        y ^= (y >> self.L)

        self.index += 1
        return y
    
    """
        Recover `x` from `y` when:
            `y = x ^ ((x << shift) & mask)` 
    """
    def _untempering_left(self, y, shift, mask):
        x = 0
        for i in range(shift, self.W, shift):
            m = (1 << i) - 1
            x = (y ^ ((x << shift) & mask)) & m

        x = (y ^ ((x << shift) & mask))
        return x & self.WORD_MASK

    """
        Recover `x` from `y` when:
            `y = x ^ ((x >> shift) & mask)` 
    """
    def _untempering_right(self, y, shift, mask):
        x = 0
        for i in range(shift, self.W, shift):
            m = ((1 << i) - 1) << (self.W - i)
            x = (y ^ ((x >> shift) & mask)) & m

        x = (y ^ ((x >> shift) & mask))
        return x & self.WORD_MASK

    """
        Generate a w-bits "random" number from MT19937 context.
    """
    def next(self):
        if self.index >= self.N:
            self._twist()
        
        y = self._tempering()
        return y