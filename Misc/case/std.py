import string
import base64
import hashlib

FLAG = 'NOCTF{WelC0m3_7o_NOC7F_2023_7h1s_1s_7he_ch3ck1n_7askk}'
std = 'fWtrc2E3X24xa2MzaGNfZWg3X3MxX3MxaDdfMzIwMl9GN0NPTl9vN18zbTBDbGVXe0ZUQ09O'

c = 'FWTRC2E3X24XA2MZAGNFZWG3X3MXX3MXADDFMZIWML9GN0NPTL9VN18ZBTBDBGVXE0ZUQ09O'
md5 = "39dc13b299c509994b5f3f533f9b1531"

S = string.ascii_letters + string.digits + '_' + '{' + '}'

# letter = string.ascii_letters
# cnt = 0
# for c in letter:
#     cnt += 1
# print(cnt)
print(2**52)


class StringCaseIterator:
    def __init__(self, s):
        self.s = s
        self.num = 2 ** len(s)

    def __iter__(self):
        self.i = 0
        return self

    def __next__(self):
        if self.i >= self.num:
            raise StopIteration
        else:
            combo = ''
            for j in range(len(self.s)):
                if (self.i >> j) % 2 == 0:
                    combo += self.s[j].upper()
                else:
                    combo += self.s[j].lower()
            self.i += 1
            return combo


flags = []
for i in range(0, len(c), 4):
    s = c[i:i + 4]
    decoded = []
    for combo in StringCaseIterator(s):
        try:
            d = base64.b64decode(combo.encode()).decode()
            if all(char in S for char in d):
                decoded.append(d)
        except:
            continue
    decoded = list(set(decoded))
    tmp = []
    if len(decoded) == 2:
        tmp.extend(flags)
        if len(tmp) == 0:
            tmp.append(decoded[1])
        else:
            tmp = [k + decoded[1] for k in tmp]
    if len(decoded) > 0:
        if len(flags) == 0:
            flags.append(decoded[0])
        else:
            flags = [k + decoded[0] for k in flags]
    flags = list(set(flags) | set(tmp))
flag = ''

for i in flags:
    if hashlib.md5(i[::-1].encode()).hexdigest() == md5:
        print(flags.index(i))
        flag = i[::-1]
        print(flag)

if flag != FLAG:
    print(flags)
    print(FLAG[::-1])
