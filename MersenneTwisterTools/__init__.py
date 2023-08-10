from .mt19937    import MT19937
from .mt19937_64 import MT19937_64

from typing import Union
import time

__version__ = "0.0.1"

def new(algorithm: str, seed=None) -> Union[MT19937, MT19937_64]:
    algorithm = algorithm.lower()
    assert algorithm in ["mt19937", "mt19937_64"], \
        "Only support `mt19937` and `mt19937_64` algorithms"

    if not seed:
        print(
            "[!] Not specifying `seed` value -> set `seed` as current time"
        )
        seed = round(time.time())

    obj = {
        "mt19937"    : MT19937,
        "mt19937_64" : MT19937_64
    }[algorithm](SEED=seed)

    return obj

def state_recovery(algorithm: str, output) -> Union[MT19937, MT19937_64]:
    algorithm = algorithm.lower()
    
    assert algorithm in ["mt19937", "mt19937_64"], \
        "Only support `mt19937` and `mt19937_64` algorithms"
    
    obj = {
        "mt19937"    : MT19937,
        "mt19937_64" : MT19937_64
    }[algorithm](SEED=0)    # dont care

    assert len(output) == obj.N

    obj.index = 0
    while obj.index < obj.N:
        y = output[obj.index]
        y = obj._untempering_right(y, obj.L, obj.WORD_MASK)
        y = obj._untempering_left(y, obj.T, obj.C)
        y = obj._untempering_left(y, obj.S, obj.B)
        y = obj._untempering_right(y, obj.U, obj.D)

        obj.state[obj.index] = y
        obj.index += 1

    return obj

__all__ = ("new", "state_recovery")