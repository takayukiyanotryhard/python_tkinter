#include "iphone_access_exports.h"
static bool initialized = false;
HMODULE dll;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    OutputDebugString(L"dllmain");
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
        if (!initialized) {
            dll = LoadLibrary(L"imobiledevice.dll");
        }
        break;
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        if (dll) FreeLibrary(dll);
        initialized = false;
        break;
    }
    return TRUE;
}

