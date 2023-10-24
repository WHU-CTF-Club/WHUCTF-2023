#include <stdio.h>
#include <stdint.h>

void encrypt(uint32_t* v, uint32_t* key) {
    uint32_t v0 = v[0], v1 = v[1];
    uint32_t sum = 0, delta = 0x9E3779B9;

    for (int i=0; i<32; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
    }

    v[0]=v0;
    v[1]=v1;
}

void decrypt(uint32_t* v, uint32_t* key) {
    uint32_t v0 = v[0], v1 = v[1];
    uint32_t delta = 0x9E3779B9;
    uint32_t sum = delta * 32;

    for (int i=0; i<32; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
    }

    v[0]=v0;
    v[1]=v1;
}

// test
int main()
{
    char flag[33] = "NOCTF{C4lCu1a7e_0nliN3_1s_F4kE!}";
    char key[17] = "Lab_Sign_1N_Key@";

    for (int i = 0; i < 32; ++i)
        flag[i] ^= 0x1F;

    encrypt((uint32_t*)(flag + 0), (uint32_t*)key);
    encrypt((uint32_t*)(flag + 8), (uint32_t*)key);
    decrypt((uint32_t*)(flag + 16), (uint32_t*)key);
    decrypt((uint32_t*)(flag + 24), (uint32_t*)key);

    for (int i = 0; i < 32; ++i)
        printf("0x%02x, ", (unsigned char)flag[i]);
    putchar('\n');

    return 0;
}