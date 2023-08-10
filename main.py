import MersenneTwisterTools as MTT
import random

out = [random.getrandbits(32) for i in range(624)]
cracker = MTT.state_recovery(algorithm="mt19937", output=out)

for i in range(1337):
    assert random.getrandbits(32) == cracker.next()