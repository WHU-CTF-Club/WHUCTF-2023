import os
import json
import random
import string
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
from Crypto.Random import get_random_bytes
from flask import Flask, request, render_template

app = Flask(__name__)

# 设置AES加密密钥，这需要保密存储
encryption_key = get_random_bytes(16)

def safe(data : str) -> bool:
    if data == "admin":
        return False

    for c in data:
        if c not in string.ascii_letters + string.digits + ".@-_!":
            return False
    return True

def safe_form(data : dict):
    for key, value in data.items():
        if isinstance(value, str) and not safe(value):
            return False
    return True

@app.route('/', methods=['GET', 'POST'])
def user_info():
    if request.method == 'POST':
        data = request.form
        user_info = {
            'name': data['name'],
            'age': data['age'],
            'phone': data['phone'],
            'email': data['email'],
            'birthday': data['birthday'],
            'qq': data['qq'],
            'qqpass': data['qqpass'],
            'cardid': data['cardid'],
            'cardpass': data['cardpass'],
        }

        if not safe_form(data):
            return "Bad charset in input. (English name only and can't be admin)"

        # tranform json to string
        user_info_str = json.dumps(user_info)

        # generate iv
        iv = get_random_bytes(16)

        # encrypt data
        cipher = AES.new(encryption_key, AES.MODE_CBC, iv)
        encrypted_info = cipher.encrypt(pad(user_info_str.encode(), AES.block_size))

        # 将IV和加密后的数据进行hex编码
        iv_hex = iv.hex()
        encrypted_info_hex = encrypted_info.hex()
        return render_template("cipher.html", iv=iv_hex, cipher=encrypted_info_hex)
    
    return render_template('index.html')

@app.route('/decrypt', methods=["POST"])
def decrypt():
    iv_hex = request.form.get('iv')
    encrypted_info_hex = request.form.get('data')

    iv = bytes.fromhex(iv_hex)
    encrypted_info = bytes.fromhex(encrypted_info_hex)

    # decrypt
    cipher = AES.new(encryption_key, AES.MODE_CBC, iv)
    decrypted_info = cipher.decrypt(encrypted_info)

    # remove padding
    decrypted_info = unpad(decrypted_info, AES.block_size)
    user_info = json.loads(decrypted_info)

    op_pics = os.listdir(os.path.join(os.path.dirname(__file__), "static", "op"))
    a_op_pic = random.choice(op_pics)

    if user_info['name'] == "admin":
        return open("flag.txt", "r").read()

    return render_template("op.html", 
                            name=user_info['name'], 
                            opname=a_op_pic.split('.')[0], 
                            imgpath=f"static/op/{a_op_pic}")

if __name__ == '__main__':
    print(app.static_folder)
    app.run(host="0.0.0.0", port=3000)
