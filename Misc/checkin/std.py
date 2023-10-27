from pwn import *

p = remote('0.0.0.0', 9695)

p.interactive()