#include <win32.h>
#include <utils.h>
#include <winternl.h>

SEC( text, B ) UINT_PTR TGetModuleHandle( UINT_PTR hModuleHash )
{
    PLDR_DATA_TABLE_ENTRY pModule       = (PLDR_DATA_TABLE_ENTRY)((PPEB)PPEB_PTR)->Ldr->InMemoryOrderModuleList.Flink;
    PLDR_DATA_TABLE_ENTRY pFirstModule  = pModule;

    do
    {
        DWORD ModuleHash = THashString( (WCHAR*)pModule->FullDllName.Buffer, pModule->FullDllName.Length );

        if (ModuleHash == hModuleHash)
            return (HMODULE)pModule->Reserved2[0];

        pModule = (PLDR_DATA_TABLE_ENTRY)pModule->Reserved1[0];
    } while ( pModule && pModule != pFirstModule );

    return INVALID_HANDLE_VALUE;
}

SEC( text, B ) UINT_PTR TGetProcAddr( UINT_PTR hModule, UINT_PTR ProcHash )
{
    PIMAGE_NT_HEADERS       ModuleNtHeader;
    PIMAGE_EXPORT_DIRECTORY ModuleExportedDirectory;
    PDWORD                  AddressOfFunctions;
    PDWORD                  AddressOfNames;
    PWORD                   AddressOfNameOrdinals;

    ModuleNtHeader          = RVA_2_VA( PIMAGE_NT_HEADERS,       hModule, ((PIMAGE_DOS_HEADER) hModule)->e_lfanew );
    ModuleExportedDirectory = RVA_2_VA( PIMAGE_EXPORT_DIRECTORY, hModule, ModuleNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress );

    AddressOfNames          = RVA_2_VA( PDWORD, hModule, ModuleExportedDirectory->AddressOfNames );
    AddressOfFunctions      = RVA_2_VA( PDWORD, hModule, ModuleExportedDirectory->AddressOfFunctions );
    AddressOfNameOrdinals   = RVA_2_VA( PWORD,  hModule, ModuleExportedDirectory->AddressOfNameOrdinals );

    for (DWORD i = 0; i < ModuleExportedDirectory->NumberOfNames; i++)
    {
        if ( THashString( (PCHAR)hModule + AddressOfNames[i], 0 ) == ProcHash )
            return RVA_2_VA( UINT_PTR, hModule, AddressOfFunctions[AddressOfNameOrdinals[i]] );
    }
}