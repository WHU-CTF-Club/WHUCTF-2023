from Crypto.Cipher import AES
import os
from pwn import xor
from Crypto.Util.Padding import pad, unpad

iv = b'T\xc0\xf5\x05\x94\xe9\x8f\xdb\x94\x86\xca\xe7.6\xf2?'
key = b'\xf2v\xad\xe48|\xd4\x92\xf2\xea?\x7f?\x9d4\xf0'


def encrypt(plaintext):
    cipher = AES.new(key, AES.MODE_OFB, iv)
    ciphertext = cipher.encrypt(plaintext)
    return ciphertext
def my_encrypt(plaintext):
    s = iv
    ciphertext = b''
    pad_len = 16 - (len(plaintext) % 16)
    plaintext += bytes([0])*(pad_len - 1) + bytes([pad_len])
    for i in range(0, len(plaintext), 16):
        cipher = AES.new(key, AES.MODE_ECB)
        stream = cipher.encrypt(s)
        ciphertext += xor(plaintext[i:i+16], stream)
        s = stream
    return ciphertext





def decrypt(ciphertext):
    cipher = AES.new(key, AES.MODE_OFB, iv)
    decrypted = cipher.decrypt(ciphertext)
    return decrypted


# plaintext = b"Hello World may.Maybemia!!!"
# ciphertext = encrypt(plaintext)
# ciphertext = my_encrypt(plaintext)
ciphertext = b'\xc1\xf6\xf9\xb0\xdfD\xaa\xe7jp\xfd<\xd3\xc7+\xfaU\xf9\xd2U]I\xf0\xc6CmK\x99\x85\xa4\x04>|\xdb<1\x18\x8dg\xa7Y\xbd7.\x08wp\xbb\xd3p\x18\x89\x8d\xa9\xdfr\xe3\xeb\xe8(\xc5d\x81Y$\xadQ:Yt~zW\xe8>\x01\xcf:\x08\x95'



decrypted = decrypt(ciphertext)
decrypted = unpad(decrypted, 16, style='x923')
# print(f"Plaintext: {plaintext}")
print(f"Ciphertext: {ciphertext}")
print(f"Decrypted: {decrypted}")
