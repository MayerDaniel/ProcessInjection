// HelloWorldInjector.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

constexpr DWORD BUFSIZE = 4096;

int main()
{
    DWORD PID;
    std::cout << "Please enter the desired PID you wish to inject into:\n";
    std::cin >> PID;
    
    HANDLE processHandle;
    std::cout << "\nAttempting to open a handle to the desired process...\n";
    processHandle = OpenProcess(PROCESS_ALL_ACCESS,                         // https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-openprocess                             
                                FALSE, 
                                PID);

    if (processHandle == NULL) {
        printf("Failed to open a process for PID %d", PID);
        return 0;
    }

    TCHAR relativePath[BUFSIZE] = TEXT("");
    TCHAR absolutePath[BUFSIZE] = TEXT("");
    SIZE_T absolutePathSize = 0;
    std::cout << "\nHandle opened, please enter the desired DLL you wish to inject:\n";
    std::wcin >> relativePath;
    if (!GetFullPathNameW(relativePath,                                         // https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfullpathnamew
         BUFSIZE, 
         absolutePath, 
         NULL)
        ) { 
        printf("Failed to find absolute path for %s", relativePath);
        return 0;
    }
    else {
        absolutePathSize = sizeof(absolutePath);
        wprintf(L"Absolute path: %s, size: %d\n", absolutePath, absolutePathSize);
    }

    LPVOID bufferAddressInTargetProcess;
    printf("\nAttempting to allocate buffer of size %d in PID %d...\n", absolutePathSize, PID);
    bufferAddressInTargetProcess = VirtualAllocEx( processHandle,               // https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualallocex
                                                   NULL, 
                                                   absolutePathSize + 200, 
                                                   MEM_COMMIT | MEM_RESERVE, 
                                                   PAGE_EXECUTE_READWRITE);

    if (!bufferAddressInTargetProcess) {
        printf("Failed to allocate buffer in PID %d\n", PID);
        return 0;
    }

    wprintf(L"Buffer allocated at address %#010x in target process\n\nAttempting to write absolute path to allocated buffer...", bufferAddressInTargetProcess);
    if (!WriteProcessMemory( processHandle,                                     // https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-writeprocessmemory
                             bufferAddressInTargetProcess,
                             absolutePath,
                             absolutePathSize,
                             NULL )
        ) {
        printf("Failed to write absolute path to allocated buffer at %d\n", bufferAddressInTargetProcess);
        return 0;
    }

    LPVOID loadLibraryAddress;
    loadLibraryAddress = (LPVOID)GetProcAddress(                                // https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getprocaddress
                                         GetModuleHandle(TEXT("KERNEL32.DLL")), // https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulehandlea
                                         "LoadLibraryW"
    );


    std::cout << "\nInjecting...\n";

    HANDLE remoteThread;
    remoteThread = CreateRemoteThread( processHandle,                           // https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createremotethread
                                       NULL,
                                       0,
                                       (LPTHREAD_START_ROUTINE)loadLibraryAddress,
                                       bufferAddressInTargetProcess,
                                       0,
                                       NULL
        );
    WaitForSingleObject(remoteThread, INFINITE);

    return 0;
    
}

