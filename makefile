ProjectName		= ShellcodeTemplate

CCX64			= x86_64-w64-mingw32-gcc
CCX86			= i686-w64-mingw32-gcc

CFLAGS			=  -Os -fno-asynchronous-unwind-tables -nostdlib
CFLAGS 			+= -fno-ident -fpack-struct=8 -falign-functions=1
CFLAGS  		+= -s -ffunction-sections -falign-jumps=1 -w
CFLAGS			+= -falign-labels=1 -fPIC -Wl,-Tscripts/Linker.ld
CFLAGS			+= -Wl,-s,--no-seh,--enable-stdcall-fixup

EXECUTABLE_X64	= bin/$(ProjectName).x64.exe
RAWBINARY_X64	= bin/$(ProjectName).x64.bin

EXECUTABLE_X86	= bin/$(ProjectName).x86.exe
RAWBINARY_X86	= bin/$(ProjectName).x86.bin

all: x64 x86

x64: clean
	@ echo "[*] Compile x64 executable..."
	
	@ nasm -f win64 src/asm/x64/asm.s -o bin/asm.x64.o
	@ $(CCX64) src/*.c bin/asm.x64.o -o $(EXECUTABLE_X64) $(CFLAGS) $(LFLAGS) -Iinclude -masm=intel

	@ echo "[*] Extract shellcode: $(RAWBINARY_X64)"
	@ python3 scripts/extract.py -f $(EXECUTABLE_X64) -o $(RAWBINARY_X64)

	@ rm $(EXECUTABLE_X64)

x86: clean
	@ echo "[*] Compile x86 executable..."

	@ nasm -f win32 src/asm/x86/asm.s -o bin/asm.x86.o
	@ $(CCX86) src/*.c bin/asm.x86.o -o $(EXECUTABLE_X86) $(CFLAGS) $(LFLAGS) -Iinclude -masm=intel

	@ echo "[*] Extract shellcode: $(RAWBINARY_X86)"
	@ python3 scripts/extract.py -f $(EXECUTABLE_X86) -o $(RAWBINARY_X86)

	@ rm $(EXECUTABLE_X86)

clean:
	@ rm -rf bin/*.o
	@ rm -rf bin/*.bin
	@ rm -rf bin/*.exe
