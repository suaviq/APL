// JAApp.cpp : Defines the entry point for the console application.
//
#include <windows.h>
#include <iostream>
#include <string>

#include "paths.h"
#include "asm_functions.h"
#include "asm_wrapper.h"


int main() {

#if _DEBUG
	std::cout << "Loading Debug DLL...\n\n";
	HINSTANCE hGetProcIDDLL = LoadLibrary(DEBUG_ASM_DLL_PATH);
#else
	std::cout << "Loading Release DLL...\n\n";
	HINSTANCE hGetProcIDDLL = LoadLibrary(ASM_DLL_PATH);
#endif

	if (!hGetProcIDDLL) {
		std::cout << "[ERROR] Could not load the dynamic library\n(Check if the path to the dll is correct)\n";
		return EXIT_FAILURE;
	}

	f_asm_f64x2 subtract_vectors = (f_asm_f64x2)GetProcAddress(hGetProcIDDLL, "subtract_vectors");
	if (!subtract_vectors) {
		std::cout << "could not locate the function `subtract_vectors`" << std::endl;
		return EXIT_FAILURE;
	}
	f_asm_f64x2 add_vectors = (f_asm_f64x2)GetProcAddress(hGetProcIDDLL, "add_vectors");
	if (!add_vectors) {
		std::cout << "could not locate the function `add_vectors`" << std::endl;
		return EXIT_FAILURE;
	}
	f_asm_f64x2 add_scalar_to_vector = (f_asm_f64x2)GetProcAddress(hGetProcIDDLL, "add_scalar_to_vector");
	if (!add_scalar_to_vector) {
		std::cout << "could not locate the function `add_scalar_to_vector`" << std::endl;
		return EXIT_FAILURE;
	}
	f_asm_f64x2 subtract_scalar_from_vector = (f_asm_f64x2)GetProcAddress(hGetProcIDDLL, "subtract_scalar_from_vector");
	if (!subtract_scalar_from_vector) {
		std::cout << "could not locate the function `subtract_scalar_from_vector`" << std::endl;
		return EXIT_FAILURE;
	}

	f_asm_f64x2 mul_vector_by_scalar = (f_asm_f64x2)GetProcAddress(hGetProcIDDLL, "mul_vector_by_scalar");
	if (!mul_vector_by_scalar) {
		std::cout << "could not locate the function `mul_vector_by_scalar`" << std::endl;
		return EXIT_FAILURE;
	}

	f_asm_f64x2 vector_wise_multiply = (f_asm_f64x2)GetProcAddress(hGetProcIDDLL, "vector_wise_multiply");
	if (!vector_wise_multiply) {
		std::cout << "could not locate the function `vector_wise_multiply`" << std::endl;
		return EXIT_FAILURE;
	}


	f_asm_f64x3 mul_vecT_by_vec = (f_asm_f64x3)GetProcAddress(hGetProcIDDLL, "mul_vecT_by_vec");
	if (!mul_vecT_by_vec) {
		std::cout << "could not locate the function `mul_vecT_by_vec`" << std::endl;
		return EXIT_FAILURE;
	}

	f_asm_f64x2 mul_vec_by_vecT = (f_asm_f64x2)GetProcAddress(hGetProcIDDLL, "mul_vec_by_vecT");
	if (!mul_vec_by_vecT) {
		std::cout << "could not locate the function `mul_vec_by_vecT`" << std::endl;
		return EXIT_FAILURE;
	}


	f_asm_f64x2 add_matrices = (f_asm_f64x2)GetProcAddress(hGetProcIDDLL, "add_matrices");
	if (!add_matrices) {
		std::cout << "could not locate the function `add_matrices`" << std::endl;
		return EXIT_FAILURE;
	}

	f_asm_f64x2 subtract_matrices = (f_asm_f64x2)GetProcAddress(hGetProcIDDLL, "subtract_matrices");
	if (!subtract_matrices) {
		std::cout << "could not locate the function `subtract_matrices`" << std::endl;
		return EXIT_FAILURE;
	}

	f_asm_f64x2 mul_matrix_by_scalar = (f_asm_f64x2)GetProcAddress(hGetProcIDDLL, "mul_matrix_by_scalar");
	if (!mul_matrix_by_scalar) {
		std::cout << "could not locate the function `mul_matrix_by_scalar`" << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "\n=====================\n";
	std::cout << "        VECTORS";
	std::cout << "\n=====================\n";
	{
		double* a = new double[4];
		double* b = new double[4];
		for (size_t i = 0; i < 4; i += 1) {
			a[i] = 2.0 * (i+1);
			b[i] = -0.5 * (i+1);
		}

		std::cout << '\n';
		subtract_vectors(a, b);
		for (size_t i = 0; i < 4; i += 1) {
			printf("%x: %lf\n", i, a[i]);
		}
		std::cout << '\n';
		add_vectors(a, b);
		for (size_t i = 0; i < 4; i += 1) {
			printf("%x: %lf\n", i, a[i]);
		}
		std::cout << '\n';
		add_scalar_to_vector(a, new double(0.5));
		for (size_t i = 0; i < 4; i += 1) {
			printf("%x: %lf\n", i, a[i]);
		}
		std::cout << '\n';
		subtract_scalar_from_vector(a, new double(0.25));
		for (size_t i = 0; i < 4; i += 1) {
			printf("%x: %lf\n", i, a[i]);
		}
		std::cout << '\n';
		mul_vector_by_scalar(a, new double(2.0));
		for (size_t i = 0; i < 4; i += 1) {
			printf("%x: %lf\n", i, a[i]);
		}
		std::cout << '\n';
		vector_wise_multiply(a, b);
		for (size_t i = 0; i < 4; i += 1) {
			printf("%x: %lf\n", i, a[i]);
		}
		std::cout << '\n';
		double* result = new double[16] { 0.0 };
		mul_vecT_by_vec(a, b, result);
		for (size_t i = 0; i < 16; i += 1) {
			printf("%lf\n", result[i]);
		}
		std::cout << '\n';

	}

	std::cout << "\n=====================\n";
	std::cout << "        MATRICES";
	std::cout << "\n=====================\n";
	{
		double* a = new double[16];
		double* b = new double[16];
		for (size_t i = 0; i < 16; i += 1) {
			a[i] = (double(i)+1);
			b[i] = (double(i)+1)/2;
		}
		for (size_t i = 0; i < 4; i += 1) {
			printf("%x >\t", i);
			for (size_t j = 0; j < 4; j += 1) {
				printf("%4f [%x]\t", a[4*i+j], &a[4*i+j]);
			}
			printf("\n");
		}
		printf("\n");
		for (size_t i = 0; i < 4; i += 1) {
			printf("%x >\t", i);
			for (size_t j = 0; j < 4; j += 1) {
				printf("%4f [%x]\t", b[4*i+j], &b[4*i+j]);
			}
			printf("\n");
		}
		printf("\n");
		add_matrices(a, b);
		for (size_t i = 0; i < 4; i += 1) {
			printf("%x >\t", i);
			for (size_t j = 0; j < 4; j += 1) {
				printf("%4f [%x]\t", a[4*i+j], &a[4*i+j]);
			}
			printf("\n");
		}
		printf("\n");
		subtract_matrices(a, b);
		for (size_t i = 0; i < 4; i += 1) {
			printf("%x >\t", i);
			for (size_t j = 0; j < 4; j += 1) {
				printf("%4f [%x]\t", a[4*i+j], &a[4*i+j]);
			}
			printf("\n");
		}
		printf("\n");
		mul_matrix_by_scalar(a, new double(0.5));
		for (size_t i = 0; i < 4; i += 1) {
			printf("%x >\t", i);
			for (size_t j = 0; j < 4; j += 1) {
				printf("%4f [%x]\t", a[4*i+j], &a[4*i+j]);
			}
			printf("\n");
		}
	}
	
	return EXIT_SUCCESS;
}
