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

int main()
{
    unsigned char answer[33] = 
    {
        0xd4, 0xe5, 0xbd, 0x70, 0x09, 0x20, 0xd2, 0xa3, 
        0x33, 0xbb, 0xb2, 0x7d, 0x47, 0x1b, 0xee, 0x74, 
        0x1c, 0xe4, 0x8b, 0x66, 0x15, 0xf5, 0x6e, 0xf2, 
        0x24, 0xbf, 0x39, 0xad, 0x62, 0x88, 0xb9, 0x71,
        0
    };

    char key[17] = "Lab_Sign_1N_Key@";

    decrypt((uint32_t*)(answer + 0), (uint32_t*)key);
    decrypt((uint32_t*)(answer + 8), (uint32_t*)key);
    encrypt((uint32_t*)(answer + 16), (uint32_t*)key);
    encrypt((uint32_t*)(answer + 24), (uint32_t*)key);

    for (int i = 0; i < 32; ++i)
        answer[i] ^= 0x1F;

    printf("%s\n", answer);
    return 0;
}
