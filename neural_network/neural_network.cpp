// JAApp.cpp : Defines the entry point for the console application.
//
#include <windows.h>
#include <iostream>

extern "C" int _stdcall MyProc1(DWORD x);

typedef int(__stdcall *f_MyProc1)(int**);

int main() {
	// PAMIETAJ ZEBY TUTAJ DAC SWOJA SCIEZKE: 
	// MOZEMY ZROBIC TAK ZE STWORZY SIE PLIK W STYLU sciezki.h
	// W KTORYM OBAJ WPISZEMY SWOJA SCIEZKE I DA SIE GO DO .gitignore
	HINSTANCE hGetProcIDDLL = LoadLibrary(L"C:\\Users\\macie\\OneDrive\\Pulpit\\studia\\apl\\APL\\x64\\Debug\\ASM_dll.dll");

	if (!hGetProcIDDLL) {
		std::cout << "[ERROR] Could not load the dynamic library\n(Check if the path to the dll is correct)\n";
		return EXIT_FAILURE;
	}

	f_MyProc1 MyProc1 = (f_MyProc1)GetProcAddress(hGetProcIDDLL, "MyProc1");
	if (!MyProc1) {
		std::cout << "[ERROR] Could not locate the function: MyProc1()\n";
		return EXIT_FAILURE;
	}

	int* x;
	// pro tip: jak sie uzyje %x w printf() to pisze liczbe w hex-ie
	// tutaj sprawdzam czy adres x jest taki sam co return MyProc1()
	printf("x addr: %x\n", &x);
	printf("x addr: %x\n", MyProc1(&x));

	return EXIT_SUCCESS;
}