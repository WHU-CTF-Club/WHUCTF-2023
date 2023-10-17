#pragma once

#include <climits>
#include "Memory.hpp"

class BlowfishEngine
{
public:
    BlowfishEngine();
    ~BlowfishEngine();
    void Submit_Key(void const * key, int length);
    int Encrypt(void const * plaintext, int length, void * cyphertext);
    int Decrypt(void const * cyphertext, int length, void * plaintext);
    enum 
    {
        DEF_KEY_LENGTH = 16,
        MIN_KEY_LENGTH = 4,
        MAX_KEY_LENGTH = 56
    };

    static BlowfishEngine** __fastcall BlowfishEngine_HookCTOR(BlowfishEngine** pThis)
    {
        *pThis = GameCreate<BlowfishEngine>();
        return pThis;
    }

    static void __fastcall BlowfishEngine_HookDTOR(BlowfishEngine** pThis)
    {
        GameDelete(*pThis);
    }
    
private:
    enum 
    {
        ROUNDS = 16,
        BYTES_PER_BLOCK = 8
    };
    void Sub_Key_Encrypt(unsigned long & left, unsigned long & right);
    void Process_Block(void const * plaintext, void * cyphertext, unsigned long const * ptable);
private:
    static unsigned long const P_Init[(int)ROUNDS + 2];
    static unsigned long const S_Init[4][UCHAR_MAX + 1];
private:
    bool IsKeyed;
    unsigned long P_Encrypt[(int)ROUNDS + 2];
    unsigned long P_Decrypt[(int)ROUNDS + 2];
    unsigned long bf_S[4][UCHAR_MAX + 1];
};
