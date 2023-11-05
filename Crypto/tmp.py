from random import seed, getrandbits

cnt = 0
a = []
while 1:
    cur = getrandbits(32)
    cnt += 1
    if cur in a:
        print(cnt)
        break
    else:
        a.append(cur)
