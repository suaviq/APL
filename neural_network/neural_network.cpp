// JAApp.cpp : Defines the entry point for the console application.
//
#include <windows.h>
#include <iostream>

#include "paths.h"
#include "asm_functions.h"


int main() {
	HINSTANCE hGetProcIDDLL = LoadLibrary(DEBUG_ASM_DLL_PATH);
	if (!hGetProcIDDLL) {
		std::cout << "[ERROR] Could not load the dynamic library\n(Check if the path to the dll is correct)\n";
		return EXIT_FAILURE;
	}

	f_subtract_vectors subtract_vectors = (f_subtract_vectors)GetProcAddress(hGetProcIDDLL, "subtract_vectors");
	if (!subtract_vectors) {
		std::cout << "could not locate the function `subtract_vectors`" << std::endl;
		return EXIT_FAILURE;
	}
	f_add_vectors add_vectors = (f_add_vectors)GetProcAddress(hGetProcIDDLL, "add_vectors");
	if (!add_vectors) {
		std::cout << "could not locate the function `add_vectors`" << std::endl;
		return EXIT_FAILURE;
	}
	f_add_scalar_to_vector add_scalar_to_vector = (f_add_scalar_to_vector)GetProcAddress(hGetProcIDDLL, "add_scalar_to_vector");
	if (!add_scalar_to_vector) {
		std::cout << "could not locate the function `add_scalar_to_vector`" << std::endl;
		return EXIT_FAILURE;
	}
	f_subtract_scalar_from_vector subtract_scalar_from_vector = (f_subtract_scalar_from_vector)GetProcAddress(hGetProcIDDLL, "subtract_scalar_from_vector");
	if (!subtract_scalar_from_vector) {
		std::cout << "could not locate the function `subtract_scalar_from_vector`" << std::endl;
		return EXIT_FAILURE;
	}


	double *a = new double[4];
	double *b = new double[4];
	for (size_t i = 0; i < 4; i += 1) {
		a[i] = 0.4;
		b[i] = 0.3;
	}

	for (size_t i = 0; i < 4; i += 1) {
		printf("%lu: %lf\n", i, a[i]);
	}
	std::cout << '\n';
	subtract_vectors(a, b);
	for (size_t i = 0; i < 4; i += 1) {
		printf("%lu: %lf\n", i, a[i]);
	}
	std::cout << '\n';
	add_vectors(a, b);
	for (size_t i = 0; i < 4; i += 1) {
		printf("%lu: %lf\n", i, a[i]);
	}
	std::cout << '\n';
	add_scalar_to_vector(a, new double(0.5));
	for (size_t i = 0; i < 4; i += 1) {
		printf("%lu: %lf\n", i, a[i]);
	}
	std::cout << '\n';
	subtract_scalar_from_vector(a, new double(0.25));
	for (size_t i = 0; i < 4; i += 1) {
		printf("%lu: %lf\n", i, a[i]);
	}
	std::cout << '\n';

	return EXIT_SUCCESS;
}