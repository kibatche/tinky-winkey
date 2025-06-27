#pragma once

#include <Windows.h>
#include <stdio.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <winternl.h>

//copy of SYSTEM_PROCESS_INFORMATION_HK from winternal.h in order to hook NtQuerySystemInformation
typedef struct _SYSTEM_PROCESS_INFORMATION_HK {
    ULONG NextEntryOffset;
    ULONG NumberOfThreads;
    BYTE Reserved1[48];
    UNICODE_STRING ImageName;
    KPRIORITY BasePriority;
    HANDLE UniqueProcessId;
    PVOID Reserved2;
    ULONG HandleCount;
    ULONG SessionId;
    PVOID Reserved3;
    SIZE_T PeakVirtualSize;
    SIZE_T VirtualSize;
    ULONG Reserved4;
    SIZE_T PeakWorkingSetSize;
    SIZE_T WorkingSetSize;
    PVOID Reserved5;
    SIZE_T QuotaPagedPoolUsage;
    PVOID Reserved6;
    SIZE_T QuotaNonPagedPoolUsage;
    SIZE_T PagefileUsage;
    SIZE_T PeakPagefileUsage;
    SIZE_T PrivatePageCount;
    LARGE_INTEGER Reserved7[6];
} SYSTEM_PROCESS_INFORMATION_HK, *PSYSTEM_PROCESS_INFORMATION_HK;

//copy of the signature of NtQuerySystemInformation
typedef NTSTATUS (WINAPI* NtQuerySystemInformationHk)
(
    __in SYSTEM_INFORMATION_CLASS SystemInformationClass,
    __inout PVOID SystemInformation,
    __in ULONG SystemInformationLength,
    __out_opt PULONG ReturnLength
);

NTSTATUS NTAPI HookedNtQuerySystemInformation(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);
VOID Mylog(void *toMyLog, int MODE, BOOL putDate);
