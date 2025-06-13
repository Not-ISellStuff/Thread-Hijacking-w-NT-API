#ifndef NT_H
#define NT_H

#include <windows.h>

typedef LONG NTSTATUS;
typedef ULONG ACCESS_MASK;
typedef void* PVOID;
typedef void* HANDLE;
typedef ULONG_PTR SIZE_T;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES {
    ULONG Length;
    HANDLE RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG Attributes;
    PVOID SecurityDescriptor;
    PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

typedef const OBJECT_ATTRIBUTES *PCOBJECT_ATTRIBUTES;

typedef struct _CLIENT_ID {
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

#define NTAPI __stdcall
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)

/////////////////////////////////////////////////

typedef NTSTATUS (NTAPI* NtOpenProcess)(
    PHANDLE ProcessHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    PCLIENT_ID ClientId
);

typedef NTSTATUS (NTAPI* NtAllocateVirtualMemory)(
    HANDLE ProcessHandle,
    PVOID* BaseAddress,
    ULONG_PTR ZeroBits,
    PSIZE_T RegionSize,
    ULONG AllocationType,
    ULONG Protect
);

typedef NTSTATUS (NTAPI* NtWriteVirtualMemory)(
    HANDLE ProcessHandle,
    PVOID BaseAddress,
    PVOID Buffer,
    ULONG BufferSize,
    PULONG NumberOfBytesWritten
);

typedef NTSTATUS (NTAPI* NtCreateThreadEx)(
    PHANDLE ThreadHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    HANDLE ProcessHandle,
    LPTHREAD_START_ROUTINE StartRoutine,
    PVOID Argument,
    ULONG CreateFlags,
    SIZE_T ZeroBits,
    SIZE_T StackSize,
    SIZE_T MaximumStackSize,
    PVOID AttributeList
);

typedef NTSTATUS (NTAPI* NtOpenThread)(
    PHANDLE ThreadHandle,
    ACCESS_MASK DesiredAccess,
    PCOBJECT_ATTRIBUTES ObjectAttributes,
    PCLIENT_ID ClientId
);

typedef NTSTATUS (NTAPI* NtSuspendThread)(
    HANDLE ThreadHandle,
    PULONG PreviousSuspendCount
);

typedef NTSTATUS (NTAPI* NtGetContextThread)(
    HANDLE ThreadHandle,
    PCONTEXT ThreadContext
);

typedef NTSTATUS (NTAPI* NtSetContextThread)(
    HANDLE ThreadHandle,
    const CONTEXT* ThreadContext
);

typedef NTSTATUS (NTAPI* NtResumeThread)(
    HANDLE ThreadHandle,
    PULONG SuspendCount
);

/////////////////////////////////////////////////

extern NtAllocateVirtualMemory NtVirtAlloc;
extern NtOpenProcess NtOpenProc;
extern NtWriteVirtualMemory NtWriteVirtMem;
extern NtCreateThreadEx NtCreateThread;

extern NtOpenThread NtOpenT;
extern NtSuspendThread NtSuspend;
extern NtGetContextThread NtGetContext;
extern NtSetContextThread NtSetContext;
extern NtResumeThread NtResume;

#endif
