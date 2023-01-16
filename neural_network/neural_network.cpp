// JAApp.cpp : Defines the entry point for the console application.
//
#include <windows.h>
#include <iostream>

#include "paths.h"

/*extern "C" int _stdcall MyProc1(DWORD x);

typedef int(__stdcall *f_MyProc1)(int**);
*/
extern "C" double* _stdcall substract_vectors(DWORDLONG x, DWORDLONG y);

typedef int(__stdcall *f_substract_vectors)(double*, double*);


int main() {
	// PAMIETAJ ZEBY TUTAJ DAC SWOJA SCIEZKE: 
	// MOZEMY ZROBIC TAK ZE STWORZY SIE PLIK W STYLU sciezki.h
	// W KTORYM OBAJ WPISZEMY SWOJA SCIEZKE I DA SIE GO DO .gitignore
	HINSTANCE hGetProcIDDLL = LoadLibrary(DEBUG_ASM_DLL_PATH);

	if (!hGetProcIDDLL) {
		std::cout << "[ERROR] Could not load the dynamic library\n(Check if the path to the dll is correct)\n";
		return EXIT_FAILURE;
	}

	/*f_MyProc1 MyProc1 = (f_MyProc1)GetProcAddress(hGetProcIDDLL, "MyProc1");
	if (!MyProc1) {
		std::cout << "[ERROR] Could not locate the function: MyProc1()\n";
		return EXIT_FAILURE;
	}

	int* x;
	// pro tip: jak sie uzyje %x w printf() to pisze liczbe w hex-ie
	// tutaj sprawdzam czy adres x jest taki sam co return MyProc1()
	printf("x addr: %x\n", &x);
	printf("x addr: %x\n", MyProc1(&x));
	*/
	f_substract_vectors substract_vectors = (f_substract_vectors)GetProcAddress(hGetProcIDDLL, "substract_vectors");
	if (!substract_vectors) {
		std::cout << "could not locate the function" << std::endl;
		return EXIT_FAILURE;
	}
	double *a[4] = { new double(0.3), new double(0.2), new double(0.3), new double(0.4) };// { 0.1, 0.1, 0.1, 0.1 };		//
	double *b[4] = { new double(0.8), new double(0.4), new double(0.7), new double(0.6) }; //{ 0.1, 0.1, 0.1, 0.1 };		//{ new double(0.1), new double(0.1), new double(0.1), new double(0.1) };
	std::cout << *a << std::endl << *b << std::endl;
	substract_vectors(*a, *b);
	std::cout << "substract_vectors() returned " << *a[0] << std::endl;



	return EXIT_SUCCESS;
}