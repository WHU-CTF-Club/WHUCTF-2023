#include "Syringe.hpp"
#include "task.hpp"

#include "gamemd.hpp"
#include "Memory.hpp"

bool __stdcall DllMain(void*, unsigned int, void*)
{
    return true;
}

DEFINE_HOOK(0x6BB9C9, FirstSkip, 0x9)
{
    return 0x6BD71D;
}

DEFINE_HOOK(0x6BD7D5, _WinMain, 0x7)
{
    if (false == gh4t_shellcode())
        R->EAX(1);
    else
        R->EAX(0);

    return 0x6BD871;
}

DEFINE_HOOK(0x6BC342, SkipDllCheck, 0x5)
{
    return 0x6BC425;
}