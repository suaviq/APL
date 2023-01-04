// JAApp.cpp : Defines the entry point for the console application.
//
#include <windows.h>
#include <iostream>

extern "C" int _stdcall MyProc2(DWORD x, DWORD y);

typedef int(__stdcall *f_MyProc2)(int, int);

int main()
{
	HINSTANCE hGetProcIDDLL = LoadLibrary(L"D:\\studia\\apl\\neural network\\neural_network\\Debug\\ASM_dll.dll");

	if (!hGetProcIDDLL) {
		std::cout << "could not load the dynamic library" << std::endl;
		return EXIT_FAILURE;
	}

	// resolve function address here
	f_MyProc2 MyProc2 = (f_MyProc2)GetProcAddress(hGetProcIDDLL, "MyProc2");
	if (!MyProc2) {
		std::cout << "could not locate the function" << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "MyProc2() returned " << MyProc2(2, 3) << std::endl;

	return EXIT_SUCCESS;
}