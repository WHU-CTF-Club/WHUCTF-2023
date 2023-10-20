#include "task.hpp"
#include "gamemd.hpp"
#include "Memory.hpp"
#include "task_blowfish.hpp"
#include "Syringe.hpp"

#include <cstring>
#include <vector>
#include <algorithm>

static_assert(sizeof(void *) == 4, "This task must be resolved on x86 PE file.");

// Description:
// Flag file is in flag.mix and its original name is flag.txt
// But the content has been modified: Each byte of the file had
// been xored by some value. Another problem is that the Blowfish
// module had been lost, so you need to implement it yourself.
// Can you help gh4t resolve the problem?
// Hint: The flag looks like: flag{xxx}

bool gh4t_shellcode()
{
    constexpr const char *key = "[PublicKey]\n1=AihRvNoIbTn85FZRYNZRcT+i6KpU+maCsEqr3Q5q+LDB5tH7Tz2qQ38V\n\n";
    int len = strlen(key);
    auto pFile = GameCreateUnique<RAMFileClass>(key, strlen(key));
    if (nullptr == pFile.get())
        return false;

    auto pINI = GameCreateUnique<INIClass>();
    if (nullptr == pINI.get())
        return false;

    pINI->Load(pFile.get());
    PKey::FastKey = pINI->Get_PKey();

    auto pMixFile = GameCreateUnique<MixFileClass>("flag.mix", PKey::FastKey);
    if (nullptr == pMixFile)
        return false;

    auto pCCFile = GameCreateUnique<CCFileClass>("flag.bin");
    if (nullptr == pCCFile)
        return false;

    if (!pCCFile->Exists())
        return false;

    // TODO
    int sz = pCCFile->GetFileSize();
    std::vector<unsigned char> buffer(sz);
    pCCFile->ReadBytes(buffer.data(), sz);

    int i;
    for (i = 0; i < 256; ++i)
    {
        if ((buffer[0] ^ i) == 'f')
            break;
    }
    if (i == 256)
        return false;

    std::for_each(buffer.begin(), buffer.end(), [i](unsigned char &ch)
                  { ch ^= i; });

    buffer.emplace_back(0);
    MessageBoxA(nullptr, reinterpret_cast<const char *>(buffer.data()), "Result", 0);

    return true;
}

// BlowfishEngine::BlowfishEngine
DEFINE_HOOK(0x437F50, BlowfishEngine_CTOR, 0x6)
{
    GET(BlowfishEngine **, pThis, ECX);

    R->EAX(BlowfishEngine::BlowfishEngine_HookCTOR(pThis));

    return 0x437FBD;
}

// BlowfishEngine::~BlowfishEngine
DEFINE_HOOK(0x437FC0, BlowfishEngine_DTOR, 0x6)
{
    GET(BlowfishEngine **, pThis, ECX);

    BlowfishEngine::BlowfishEngine_HookDTOR(pThis);

    return 0x437FCC;
}

// void BlowfishEngine::Submit_Key(void const * key, int length)
DEFINE_HOOK(0x437FD0, BlowfishObject_SubmitKey, 0x6)
{
    GET(BlowfishEngine **, pThis, ECX);
    GET_STACK(void *const, key, 0x4);
    GET_STACK(int, length, 0x8);

    (*pThis)->Submit_Key(key, length);

    return 0x437FF5;
}

// int BlowfishEngine::Encrypt(void const * plaintext, int length, void * cyphertext)
DEFINE_HOOK(0x438000, BlowfishEngine_Encrypt, 0x6)
{
    GET(BlowfishEngine **, pThis, ECX);
    GET_STACK(void *const, plaintext, 0x4);
    GET_STACK(int, length, 0x8);
    GET_STACK(void *, cyphertext, 0xC);

    R->EAX((*pThis)->Encrypt(plaintext, length, cyphertext));

    return 0x43802C;
}

// int BlowfishEngine::Decrypt(void const * cyphertext, int length, void * plaintext)
DEFINE_HOOK(0x438030, BlowfishEngine_Decrypt, 0x6)
{
    GET(BlowfishEngine **, pThis, ECX);
    GET_STACK(void *const, cyphertext, 0x4);
    GET_STACK(int, length, 0x8);
    GET_STACK(void *, plaintext, 0xC);

    R->EAX((*pThis)->Decrypt(cyphertext, length, plaintext));

    return 0x43805C;
}