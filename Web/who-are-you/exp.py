import re
import json
import requests
from Crypto.Cipher import AES

url = "http://127.0.0.1:3000"
burp_proxy = {"http": "http://127.0.0.1:8080"}

payload = {
    'name': "admin",
    'age': "",
    'phone': "",
    'email': "",
    'birthday': "",
    'qq': "",
    'qqpass': "",
    'cardid': "",
    'cardpass': "",
}


def xor(b1: bytes, b2: bytes) -> bytes:
    result = bytearray(b1)
    for i, b in enumerate(b2):
        result[i] ^= b
    return bytes(result)


def get_encrypt_data():
    payload['name'] = 'x' * len("admin")
    res = requests.post(url, data=payload, proxies=burp_proxy)
    encrypt_data = re.findall(r'value="(.*?)"', res.text)
    return encrypt_data[0], encrypt_data[1]


def craft_iv(iv, data):
    bytes_iv = bytes.fromhex(iv)
    assert len(bytes_iv) == AES.block_size

    new_bytes_iv1 = xor(bytes_iv, json.dumps(payload)[:AES.block_size].encode())
    payload['name'] = "admin"
    assert json.dumps(payload)[:AES.block_size] == '{"name": "admin"'
    new_bytes_iv2 = xor(new_bytes_iv1, json.dumps(payload)[:AES.block_size].encode())

    postdata = {
        'iv': new_bytes_iv2.hex(),
        'data': data
    }
    res = requests.post(url + "/decrypt", data=postdata)
    print(res.text)


if __name__ == "__main__":
    # print(AES.block_size)
    # print(json.dumps(payload)[:AES.block_size])
    iv, data = get_encrypt_data()
    craft_iv(iv, data)
