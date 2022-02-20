
#include <windows.h>
#include <macros.h>

UINT_PTR GetRIP( VOID );

typedef struct {

    struct {
        WIN32_FUNC( MessageBoxA );
        WIN32_FUNC( LoadLibraryA );
    } Win32; 

    struct {
        // Basics
        HMODULE     Kernel32;
        HMODULE     Ntdll;

        HMODULE     User32;
    } Modules;

} INSTANCE, *PINSTANCE;