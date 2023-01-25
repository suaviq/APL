#pragma once
#include <windows.h>
#include <iostream>

#include "paths.h"

// Function Types
typedef void(__stdcall* f_asm_f64x2)(double*, double*);
typedef void(__stdcall* f_asm_f64x3)(double*, double*, double*);

// Function Declarations
extern "C" double* _stdcall add_vectors(DWORDLONG x, DWORDLONG y);
extern "C" double* _stdcall subtract_vectors(DWORDLONG x, DWORDLONG y);
extern "C" double* _stdcall add_scalar_to_vector(DWORDLONG x, DWORDLONG y);
extern "C" double* _stdcall subtract_scalar_from_vector(DWORDLONG x, DWORDLONG y);
extern "C" double* _stdcall vector_wise_multiply(DWORDLONG x, DWORDLONG y);
extern "C" double* _stdcall mul_vecT_by_vec(DWORDLONG x, DWORDLONG y, DWORDLONG res);
extern "C" double* _stdcall mul_vector_by_scalar(DWORDLONG x, DWORDLONG y);
extern "C" double* _stdcall mul_matrix_by_scalar(DWORDLONG x, DWORDLONG y);
extern "C" double* _stdcall add_matrices(DWORDLONG x, DWORDLONG y);
extern "C" double* _stdcall subtract_matrices(DWORDLONG x, DWORDLONG y);

/*
 * This class wraps the asm linear algebra procedures in its member functions
 */
class AsmWrapper {

private:
	// DLL Instance
	HINSTANCE hGetProcIDDLL;

public:
	// DLL Procedures
	f_asm_f64x2 add_vectors;
	f_asm_f64x2 subtract_vectors;
	f_asm_f64x2 add_scalar_to_vector;
	f_asm_f64x2 subtract_scalar_from_vector;
	f_asm_f64x2 vector_wise_multiply;
	f_asm_f64x3 mul_vecT_by_vec;
	f_asm_f64x2 mul_vector_by_scalar;
	f_asm_f64x2 mul_matrix_by_scalar;
	f_asm_f64x2 add_matrices;
	f_asm_f64x2 subtract_matrices;
	double* mul_matrix_by_vec(double* m, double* v);

	// Constructor & Destructor
	AsmWrapper();
	~AsmWrapper() = default;

};

AsmWrapper::AsmWrapper() {
#if _DEBUG
	std::cout << "Loading Debug DLL...\n\n";
	HINSTANCE hGetProcIDDLL = LoadLibrary(DEBUG_ASM_DLL_PATH);
#else
	std::cout << "Loading Release DLL...\n\n";
	HINSTANCE hGetProcIDDLL = LoadLibrary(ASM_DLL_PATH);
#endif
	
	// Check if DLL Exists
	if (!hGetProcIDDLL) {
		std::cerr << "[ERROR] Could not load the dynamic library\n(Check if the path to the dll is correct)\n";
		exit( EXIT_FAILURE );
	}

	// Assign procedures from DLL
	subtract_vectors = (f_asm_f64x2)GetProcAddress(hGetProcIDDLL, "subtract_vectors");
	if (!subtract_vectors) {
		std::cerr << "[ERROR] could not locate the function `subtract_vectors`" << std::endl;
		exit( EXIT_FAILURE );
	}
	add_vectors = (f_asm_f64x2)GetProcAddress(hGetProcIDDLL, "add_vectors");
	if (!add_vectors) {
		std::cerr << "[ERROR] could not locate the function `add_vectors`" << std::endl;
		exit( EXIT_FAILURE );
	}
	add_scalar_to_vector = (f_asm_f64x2)GetProcAddress(hGetProcIDDLL, "add_scalar_to_vector");
	if (!add_scalar_to_vector) {
		std::cerr << "[ERROR] could not locate the function `add_scalar_to_vector`" << std::endl;
		exit( EXIT_FAILURE );
	}
	subtract_scalar_from_vector = (f_asm_f64x2)GetProcAddress(hGetProcIDDLL, "subtract_scalar_from_vector");
	if (!subtract_scalar_from_vector) {
		std::cerr << "[ERROR] could not locate the function `subtract_scalar_from_vector`" << std::endl;
		exit( EXIT_FAILURE );
	}
	mul_vector_by_scalar = (f_asm_f64x2)GetProcAddress(hGetProcIDDLL, "mul_vector_by_scalar");
	if (!mul_vector_by_scalar) {
		std::cerr << "[ERROR] could not locate the function `mul_vector_by_scalar`" << std::endl;
		exit( EXIT_FAILURE );
	}
	vector_wise_multiply = (f_asm_f64x2)GetProcAddress(hGetProcIDDLL, "vector_wise_multiply");
	if (!vector_wise_multiply) {
		std::cerr << "[ERROR] could not locate the function `vector_wise_multiply`" << std::endl;
		exit( EXIT_FAILURE );
	}
	mul_vecT_by_vec = (f_asm_f64x3)GetProcAddress(hGetProcIDDLL, "mul_vecT_by_vec");
	if (!mul_vecT_by_vec) {
		std::cerr << "[ERROR] could not locate the function `mul_vecT_by_vec`" << std::endl;
		exit( EXIT_FAILURE );
	}
	add_matrices = (f_asm_f64x2)GetProcAddress(hGetProcIDDLL, "add_matrices");
	if (!add_matrices) {
		std::cerr << "[ERROR] could not locate the function `add_matrices`" << std::endl;
		exit( EXIT_FAILURE );
	}
	subtract_matrices = (f_asm_f64x2)GetProcAddress(hGetProcIDDLL, "subtract_matrices");
	if (!subtract_matrices) {
		std::cerr << "[ERROR] could not locate the function `subtract_matrices`" << std::endl;
		exit( EXIT_FAILURE );
	}
	mul_matrix_by_scalar = (f_asm_f64x2)GetProcAddress(hGetProcIDDLL, "mul_matrix_by_scalar");
	if (!mul_matrix_by_scalar) {
		std::cerr << "[ERROR] could not locate the function `mul_matrix_by_scalar`" << std::endl;
		exit( EXIT_FAILURE );
	}
}

double* AsmWrapper::mul_matrix_by_vec(double* m, double* v) {
	double* ret = new double[4] { 0.0 };
	return ret;
}
