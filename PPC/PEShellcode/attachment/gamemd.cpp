#include "gamemd.hpp"

__declspec(naked) int __stdcall MessageBoxA(void* hWnd, const char* lpText, const char* lpCaption, unsigned int uType)
{
    __asm
    {
        jmp dword ptr ds:0x7E1460
    }
}

__declspec(naked) int __fastcall Set_Required_CD(int cd)
{
    __asm
    {
        mov eax, 0x4790B0
        jmp eax
    }
}

__declspec(naked) MixFileClass::MixFileClass(const char* pFileName, const PKey& key)
{
    __asm
    {
        mov eax, 0x5B3C20
        jmp eax
    }
}

__declspec(naked) MixFileClass::~MixFileClass()
{
    __asm
    {
        mov eax, 0x5B3F50
        jmp eax
    }
}

__declspec(naked) RAMFileClass::RAMFileClass(const void* data, int length)
{
    __asm
    {
        mov eax, 0x65C250
        jmp eax
    }
}

__declspec(naked) RAMFileClass::~RAMFileClass()
{
    __asm
    {
        mov eax, 0x65C2A0
        jmp eax
    }
}

__declspec(naked) CCFileClass::CCFileClass()
{
    __asm
    {
        mov eax, 0x473A80
        jmp eax
    }
}

__declspec(naked) CCFileClass::CCFileClass(const char* pFileName)
{
    __asm
    {
        mov eax, 0x4739F0
        jmp eax
    }
}

__declspec(naked) CCFileClass::~CCFileClass()
{
    __asm
    {
        mov eax, 0x535A70
        jmp eax
    }
}

__declspec(naked) unsigned int CRCEngine::operator()(void* buffer, int length)
{
    __asm
    {
        mov eax, 0x4A1DE0
        jmp eax
    }
}

__declspec(naked) INIClass::INIClass()
{
    __asm
    {
        mov eax, 0x535AA0
        jmp eax
    }
}

__declspec(naked) INIClass::~INIClass()
{
    __asm
    {
        mov eax, 0x5256F0
        jmp eax
    }
}

__declspec(naked) int INIClass::Load(RAMFileClass* file, bool flag)
{
    __asm
    {
        mov eax, 0x525A10
        jmp eax
    }
}

__declspec(naked) PKey INIClass::Get_PKey(bool fast)
{
    __asm
    {
        mov eax, 0x52A670
        jmp eax
    }
}