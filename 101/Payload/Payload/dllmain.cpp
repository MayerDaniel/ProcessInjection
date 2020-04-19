// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    // NOTE: 
    // You can execute code here as well, and it will execute
    // regardless of the value of ul_reason_for_call

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        MessageBox(
            NULL,
            (LPCWSTR)L"DLL_PROCESS_ATTACH",
            (LPCWSTR)L"In DllMain",
            MB_OK
        );
    case DLL_THREAD_ATTACH:
        MessageBox(
            NULL,
            (LPCWSTR)L"DLL_THREAD_ATTACH",
            (LPCWSTR)L"In DllMain",
            MB_OK
        );
    case DLL_THREAD_DETACH:
        MessageBox(
            NULL,
            (LPCWSTR)L"DLL_THREAD_DETACH",
            (LPCWSTR)L"In DllMain",
            MB_OK
        );
    case DLL_PROCESS_DETACH:
        MessageBox(
            NULL,
            (LPCWSTR)L"DLL_PROCESS_DETACH",
            (LPCWSTR)L"In DllMain",
            MB_OK
        );
        break;
    }

    // As well as here, after the switch statement

    return TRUE;
}



