// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    BYTE* stringAddr = (BYTE*)GetModuleHandle(TEXT("HelloWorld.exe")) + 0x1CB30;
    LPVOID stringAddress = LPVOID(stringAddr);
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        
        DWORD oldPerms;
        if (!VirtualProtect(stringAddress,
            0xE,
            PAGE_EXECUTE_READWRITE, 
            &oldPerms  
        )) {
            printf("Error with VirtualProtect! %#010x", GetLastError());
            exit(0);
        }

        //write to our new buffer
        if (!WriteProcessMemory( GetCurrentProcess(),
            stringAddress,
            "Injected!\n",
            0xc,
            NULL
        )) {
            printf("Error with VirtualProtect! %#010x", GetLastError());
            exit(0);
        }

        DWORD finalPerms;
        if (!VirtualProtect(stringAddress,
            0xE,
            oldPerms, 
            &finalPerms 
        )) {
            printf("Error with VirtualProtect! %#010x", GetLastError());
            exit(0);
        }
  
    case DLL_THREAD_ATTACH:
        
    case DLL_THREAD_DETACH:
        
    case DLL_PROCESS_DETACH:
        
        break;

    }


    return TRUE;
}



