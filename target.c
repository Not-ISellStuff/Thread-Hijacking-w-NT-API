#define UNICODE
#define _UNICODE
#include <windows.h>
#include <tlhelp32.h>
#include <wchar.h>

#include "nt.h"

DWORD Target(const wchar_t* TargetProcesses[], int Count) {

        DWORD  ThreadID  = 0;
        HANDLE HSnapShot = NULL;

        HSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (HSnapShot == INVALID_HANDLE_VALUE) { return ThreadID; }

        PROCESSENTRY32 Entry = {0};
        Entry.dwSize = sizeof(PROCESSENTRY32);

        DWORD PID = 0;

        if (Process32First(HSnapShot, &Entry)) {
            do {
                for (int i = 0; i < Count; i++) { 
                    if (_wcsicmp(Entry.szExeFile, TargetProcesses[i]) == 0) { 
                        PID = Entry.th32ProcessID;
                        break;
                    }
                }
            } while (PID == 0 && Process32Next(HSnapShot, &Entry));
        }

        CloseHandle(HSnapShot);
        if (!PID) { return ThreadID; }

        /////////////////////////////////////////////////

        HSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        if (HSnapShot == INVALID_HANDLE_VALUE) { return ThreadID; }

        THREADENTRY32 TEntry = {0};
        TEntry.dwSize = sizeof(THREADENTRY32);

        if (Thread32First(HSnapShot, &TEntry)) {
            do {
                if (TEntry.th32OwnerProcessID == PID) {
                    ThreadID = TEntry.th32ThreadID;
                    break;
                }
            } while (Thread32Next(HSnapShot, &TEntry));
        }

        CloseHandle(HSnapShot);
        return ThreadID;
}
