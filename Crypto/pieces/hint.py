from os import urandom
from random import getrandbits, seed

_seed = urandom(32)
seed(_seed)
print(hex(getrandbits(64)))
print(hex(getrandbits(64)))
seed(_seed)
print(hex(getrandbits(32)))
print(hex(getrandbits(32)))
print(hex(getrandbits(32)))
print(hex(getrandbits(32)))

