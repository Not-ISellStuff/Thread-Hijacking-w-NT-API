#ifndef MAIN_H
#define MAIN_H
#include <windows.h>

DWORD Target(const wchar_t* TargetProcesses[], int Count);

LPVOID Load(DWORD Thread, BYTE *Payload, SIZE_T PayloadSize, HMODULE nt);

BOOL Hijack(LPVOID RemoteAddr, DWORD Thread);

#endif
