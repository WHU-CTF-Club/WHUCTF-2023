from hashlib import sha256
import socketserver
import signal
import string
import random
import time

table = string.ascii_letters + string.digits
BANNER = br'''
 ,--,                                                 
,--.'|                                                 
|  | :            ,--,      ,---,              __  ,-. 
:  : '          ,'_ /|  ,-+-. /  |           ,' ,'/ /| 
|  ' |     .--. |  | : ,--.'|'   |  ,--.--.  '  | |' | 
'  | |   ,'_ /| :  . ||   |  ,"' | /       \ |  |   ,' 
|  | :   |  ' | |  . .|   | /  | |.--.  .-. |'  :  /   
'  : |__ |  | ' |  | ||   | |  | | \__\/: . .|  | '    
|  | '.'|:  | : ;  ; ||   | |  |/  ," .--.; |;  : |    
;  :    ;'  :  `--'   \   | |--'  /  /  ,.  ||  , ;    
|  ,   / :  ,      .-./   |/     ;  :   .'   \---'     
 ---`-'   `--`----'   '---'      |  ,     .-./         
                                  `--`---'             
'''

guard_menu = br'''
1.Tell me her position
2.Go away
'''

with open("flag", "rb") as f:
    FLAG = f.read()


class Task(socketserver.BaseRequestHandler):

    def _recvall(self):
        BUFF_SIZE = 2048
        data = b''
        while True:
            part = self.request.recv(BUFF_SIZE)
            data += part
            if len(part) < BUFF_SIZE:
                break
        return data.strip()

    def send(self, msg, newline=True):
        try:
            if newline:
                msg += b'\n'
            self.request.sendall(msg)
        except:
            pass

    def recv(self, prompt=b'[-] '):
        self.send(prompt, newline=False)
        return self._recvall()

    def proof_of_work(self):
        proof = (''.join([random.choice(table) for _ in range(12)])).encode()
        sha = sha256(proof).hexdigest().encode()
        self.send(b"[+] sha256(XXXX+" + proof[4:] + b") == " + sha)
        XXXX = self.recv(prompt=b'[+] Give Me XXXX :')
        if len(XXXX) != 4 or sha256(XXXX + proof[4:]).hexdigest().encode() != sha:
            return False
        return True

    def register(self):
        self.send(b'')
        username = self.recv()
        return username

    def check(self, location):
        east_longitude = b'151.19'  # +-5
        south_latitude = b'-33.888'
        try:
            latitude, longitude = location.split(b',')
            if latitude[:7] == south_latitude and longitude[:6] == east_longitude:
                x = int(longitude[6:8])
                y = 0
                if 75 <= int(longitude[6:8]) <= 85:
                    return True
                else:
                    return False
            return False
        except:
            return False

    def handle(self):
        # signal.alarm(120)
        # time.sleep(60)
        if not self.proof_of_work():
            return
        self.send(BANNER, newline=False)
        self.send(b"You have two choices. Tell me where she is or leave now.")
        self.send(guard_menu, newline=False)
        option = self.recv()
        if option == b'1':
            try:
                self.send(b"Please give me Latitude and longitude that I can directly to search in the google map: ")
                location = self.register().strip()

                if self.check(location):
                    self.send(b"Unbelievable! How did you get it!")
                    self.send(b"This is my award:")
                    self.send(FLAG)
                else:
                    self.send(b"You are a bad wingman.")
            except:
                self.request.close()
        if option == b'2':
            self.send(b"Stay away from here!")
        self.request.close()


class ForkedServer(socketserver.ForkingMixIn, socketserver.TCPServer):
    pass


if __name__ == "__main__":
    HOST, PORT = '0.0.0.0', 1338
    server = ForkedServer((HOST, PORT), Task)
    server.allow_reuse_address = True
    server.serve_forever()