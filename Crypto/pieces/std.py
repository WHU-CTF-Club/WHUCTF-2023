import random

from extend_mt19937_predictor import ExtendMT19937Predictor
from Crypto.Util.number import long_to_bytes
from Crypto.Cipher import AES

predictor = ExtendMT19937Predictor(check=True)

n_list = []
n1_list = []
with open('task.txt', 'r') as f:
    for i in range(312):
        b = ""
        for j in range(4):
            b = f.readline().strip() + b
            if j == 3:
                n_list.append(int(b, 16))
    for i in range(156):
        n_list.append(int(f.readline(), 16))
    a = []
    b = []
    for i in range(78):
        a.append(int(f.readline(), 16))
    for i in range(78):
        b.append(int(f.readline(), 16))
    for i in range(78):
        n1_list.append(a[i] | b[i])


for i in n_list:
    predictor.setrandbits(i, 32)
for i in n1_list:
    predictor.setrandbits(i, 64)

key = predictor.predict_getrandbits(128)
iv = predictor.predict_getrandbits(128)
key = long_to_bytes(key)[::-1]
iv = long_to_bytes(iv)[::-1]

ct = bytes.fromhex("e62e9a0250874f07d8fff26c88c2c5c6f92394db8f71058fa9f862cf02a0408031b301b2b92410fffc041b6c619dbc12")
cipher = AES.new(key, AES.MODE_CBC, iv)
flag = cipher.decrypt(ct)
print(flag)

random.choice