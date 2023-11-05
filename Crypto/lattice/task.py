from random import randint
from gmpy2 import gcd, invert
from Crypto.Util.number import bytes_to_long, long_to_bytes
from flag import flag


def genKey(_n):
    _privKey = [randint(1, pow(4, _n))]
    s = _privKey[0]
    for i in range(1, _n):
        _privKey.append(randint(s + 1, pow(4, _n + i)))
        s += _privKey[i]

    _q = randint(_privKey[_n - 1] + 1, 2 * _privKey[_n - 1])

    _r = randint(1, _q)
    while gcd(_r, _q) != 1:
        _r = randint(1, _q)

    _pubKey = [_r * _ % _q for _ in _privKey]
    return _privKey, _q, _r, _pubKey


def encrypt(_msg, _pubKey):
    cipher = 0
    cnt = 0
    for bit in _msg:
        cipher += int(bit) * _pubKey[cnt]
        cnt += 1
    return bin(cipher)[2:]


def decrypt(_c, _privKey, _q, _r):
    s = invert(_r, _q)
    _msg = int(_c, 2) * s % _q

    res = ''
    for i in range(len(_privKey) - 1, -1, -1):
        if _msg >= _privKey[i]:
            res = '1' + res
            _msg -= _privKey[i]
        else:
            res = '0' + res
    return res


bin_msg = bin(bytes_to_long(flag))[2:]
private_key, q, r, public_key = genKey(len(bin_msg))
enc = encrypt(bin_msg, public_key)
print(f'pubKey = {public_key}')
print(f'enc = {int(enc, 2)}')
