#pragma once
#include <windows.h>
#include <iostream>

#include "paths.h"

// Function Types
typedef void(__stdcall* f_asm_f64x2)(double*, double*);
typedef void(__stdcall* f_asm_f64x3)(double*, double*, double*);

// Function Declarations (TODO: this `double*` could be changed to void (perhaps these lines could be deleted?))
extern "C" double* _stdcall _add_vectors(DWORDLONG x, DWORDLONG y, DWORDLONG result);
extern "C" double* _stdcall _subtract_vectors(DWORDLONG x, DWORDLONG y, DWORDLONG result);
extern "C" double* _stdcall _add_scalar_to_vector(DWORDLONG x, DWORDLONG y, DWORDLONG result);
extern "C" double* _stdcall _subtract_scalar_from_vector(DWORDLONG x, DWORDLONG y, DWORDLONG result);
extern "C" double* _stdcall _vector_wise_multiply(DWORDLONG x, DWORDLONG y, DWORDLONG result);
extern "C" double* _stdcall _mul_vecT_by_vec(DWORDLONG x, DWORDLONG y, DWORDLONG result);
extern "C" double* _stdcall _mul_vec_by_vecT(DWORDLONG x, DWORDLONG y, DWORDLONG result);
extern "C" double* _stdcall _relu_vec(DWORDLONG x, DWORDLONG result);
extern "C" double* _stdcall _derivative_relu_vec(DWORDLONG x, DWORDLONG y, DWORDLONG result);
extern "C" double* _stdcall _mul_vector_by_scalar(DWORDLONG x, DWORDLONG y, DWORDLONG result);
extern "C" double* _stdcall _mul_matrix_by_scalar(DWORDLONG x, DWORDLONG y, DWORDLONG result);
extern "C" double* _stdcall _add_matrices(DWORDLONG x, DWORDLONG y, DWORDLONG result);
extern "C" double* _stdcall _subtract_matrices(DWORDLONG x, DWORDLONG y, DWORDLONG result);

/*
 * This class wraps the asm linear algebra procedures in its member functions
 */
class AsmWrapper {

private:
	// DLL Instance
	HINSTANCE hGetProcIDDLL;
	// DLL Procedures
	f_asm_f64x3 _add_vectors;
	f_asm_f64x3 _subtract_vectors;
	f_asm_f64x3 _add_scalar_to_vector;
	f_asm_f64x3 _subtract_scalar_from_vector;
	f_asm_f64x3 _vector_wise_multiply;
	f_asm_f64x3 _mul_vecT_by_vec;
	f_asm_f64x3 _mul_vec_by_vecT;
	f_asm_f64x2 _relu_vec;
	f_asm_f64x3 _derivative_relu_vec;
	f_asm_f64x3 _mul_vector_by_scalar;
	f_asm_f64x3 _mul_matrix_by_scalar;
	f_asm_f64x3 _add_matrices;
	f_asm_f64x3 _subtract_matrices;

public:
	// Accessible functions
	inline double* add_vectors(double* v1, double* v2);
	inline double* subtract_vectors(double* v1, double* v2);
	inline double* add_scalar_to_vector(double* v, double* s);
	inline double* subtract_scalar_from_vector(double* v, double* s);
	inline double* vector_wise_multiply(double* v1, double* v2);
	inline double* mul_vecT_by_vec(double* vT, double* v);
	inline double  mul_vec_by_vecT(double* v, double* vT); // TODO: check if this can be non-pointer
	inline double* relu_vec(double* v);
	inline double* derivative_relu_vec(double* v);
	inline double* mul_vector_by_scalar(double* v, double* s);
	inline double* mul_matrix_by_scalar(double* m, double* s);
	inline double* add_matrices(double* m1, double* m2);
	inline double* subtract_matrices(double* m1, double* m2);
	inline double* mul_matrix_by_vec(double* m, double* v);

	// Constructor & Destructor
	AsmWrapper();
	~AsmWrapper() = default;

};

AsmWrapper::AsmWrapper() {
#if _DEBUG
	std::cout << "Loading Debug DLL...\n";
	HINSTANCE hGetProcIDDLL = LoadLibrary(DEBUG_ASM_DLL_PATH);
#else
	std::cout << "Loading Release DLL...\n";
	HINSTANCE hGetProcIDDLL = LoadLibrary(ASM_DLL_PATH);
#endif
	
	// Check if DLL Exists
	if (!hGetProcIDDLL) {
		std::cerr << "[ERROR] Could not load the dynamic library\n(Check if the path to the dll is correct)\n";
		exit( EXIT_FAILURE );
	}

	// Assign procedures from DLL
	_subtract_vectors = (f_asm_f64x3)GetProcAddress(hGetProcIDDLL, "subtract_vectors");
	if (!_subtract_vectors) {
		std::cerr << "[ERROR] could not locate the function `subtract_vectors`" << std::endl;
		exit( EXIT_FAILURE );
	}
	_add_vectors = (f_asm_f64x3)GetProcAddress(hGetProcIDDLL, "add_vectors");
	if (!_add_vectors) {
		std::cerr << "[ERROR] could not locate the function `add_vectors`" << std::endl;
		exit( EXIT_FAILURE );
	}
	_add_scalar_to_vector = (f_asm_f64x3)GetProcAddress(hGetProcIDDLL, "add_scalar_to_vector");
	if (!_add_scalar_to_vector) {
		std::cerr << "[ERROR] could not locate the function `add_scalar_to_vector`" << std::endl;
		exit( EXIT_FAILURE );
	}
	_subtract_scalar_from_vector = (f_asm_f64x3)GetProcAddress(hGetProcIDDLL, "subtract_scalar_from_vector");
	if (!_subtract_scalar_from_vector) {
		std::cerr << "[ERROR] could not locate the function `subtract_scalar_from_vector`" << std::endl;
		exit( EXIT_FAILURE );
	}
	_mul_vector_by_scalar = (f_asm_f64x3)GetProcAddress(hGetProcIDDLL, "mul_vector_by_scalar");
	if (!_mul_vector_by_scalar) {
		std::cerr << "[ERROR] could not locate the function `mul_vector_by_scalar`" << std::endl;
		exit( EXIT_FAILURE );
	}
	_vector_wise_multiply = (f_asm_f64x3)GetProcAddress(hGetProcIDDLL, "vector_wise_multiply");
	if (!_vector_wise_multiply) {
		std::cerr << "[ERROR] could not locate the function `vector_wise_multiply`" << std::endl;
		exit( EXIT_FAILURE );
	}
	_mul_vecT_by_vec = (f_asm_f64x3)GetProcAddress(hGetProcIDDLL, "mul_vecT_by_vec");
	if (!_mul_vecT_by_vec) {
		std::cerr << "[ERROR] could not locate the function `mul_vecT_by_vec`" << std::endl;
		exit( EXIT_FAILURE );
	}
	_mul_vec_by_vecT = (f_asm_f64x3)GetProcAddress(hGetProcIDDLL, "mul_vec_by_vecT");
	if (!_mul_vec_by_vecT) {
		std::cerr << "[ERROR] could not locate the function `mul_vec_by_vecT`" << std::endl;
		exit( EXIT_FAILURE );
	}
	_relu_vec = (f_asm_f64x2)GetProcAddress(hGetProcIDDLL, "relu_vec");
	if (!_relu_vec) {
		std::cerr << "[ERROR] could not locate the function `relu_vec`" << std::endl;
		exit( EXIT_FAILURE );
	}
	_derivative_relu_vec = (f_asm_f64x3)GetProcAddress(hGetProcIDDLL, "derivative_relu_vec");
	if (!_derivative_relu_vec) {
		std::cerr << "[ERROR] could not locate the function `derivative_relu_vec`" << std::endl;
		exit( EXIT_FAILURE );
	}
	_add_matrices = (f_asm_f64x3)GetProcAddress(hGetProcIDDLL, "add_matrices");
	if (!_add_matrices) {
		std::cerr << "[ERROR] could not locate the function `add_matrices`" << std::endl;
		exit( EXIT_FAILURE );
	}
	_subtract_matrices = (f_asm_f64x3)GetProcAddress(hGetProcIDDLL, "subtract_matrices");
	if (!_subtract_matrices) {
		std::cerr << "[ERROR] could not locate the function `subtract_matrices`" << std::endl;
		exit( EXIT_FAILURE );
	}
	_mul_matrix_by_scalar = (f_asm_f64x3)GetProcAddress(hGetProcIDDLL, "mul_matrix_by_scalar");
	if (!_mul_matrix_by_scalar) {
		std::cerr << "[ERROR] could not locate the function `mul_matrix_by_scalar`" << std::endl;
		exit( EXIT_FAILURE );
	}

	std::cout << "DLL Loaded succesfully!\n";
}


inline double* AsmWrapper::add_vectors(double* v1, double* v2) {
	double* ret = new double[4];
	_add_vectors(v1, v2, ret);
	return ret;
}

inline double* AsmWrapper::subtract_vectors(double* v1, double* v2) {
	double* ret = new double[4];
	_subtract_vectors(v1, v2, ret);
	return ret;
}

inline double* AsmWrapper::add_scalar_to_vector(double* v, double* s) {
	double* ret = new double[4];
	_add_scalar_to_vector(v, s, ret);
	return ret;
}

inline double* AsmWrapper::subtract_scalar_from_vector(double* v, double* s) {
	double* ret = new double[4];
	_subtract_scalar_from_vector(v, s, ret);
	return ret;
}

inline double* AsmWrapper::vector_wise_multiply(double* v1, double* v2) {
	double* ret = new double[4];
	_vector_wise_multiply(v1, v2, ret);
	return ret;
}

inline double* AsmWrapper::mul_vecT_by_vec(double* vT, double* v) {
	double* ret = new double[16];
	_mul_vecT_by_vec(vT, v, ret);
	return ret;
}

// TODO: check if this can be non-pointer
inline double AsmWrapper::mul_vec_by_vecT(double* vT, double* v) {
	double* ret = new double[4];
	_mul_vec_by_vecT(vT, v, ret);
	return ret[0];
}

inline double* AsmWrapper::relu_vec(double* v) {
	double* ret = new double[4];
	_relu_vec(v, ret);
	return ret;
}

inline double* AsmWrapper::derivative_relu_vec(double* v) {
	double* ret = new double[4];
	_derivative_relu_vec(v, new double[4] {1.0, 1.0, 1.0, 1.0}, ret);
	return ret;
}

inline double* AsmWrapper::mul_vector_by_scalar(double* v, double* s) {
	double* ret = new double[4];
	_mul_vector_by_scalar(v, s, ret);
	return ret;
}

inline double* AsmWrapper::mul_matrix_by_scalar(double* m, double* s) {
	double* ret = new double[16];
	_mul_matrix_by_scalar(m, s, ret);
	return ret;
}

inline double* AsmWrapper::add_matrices(double* m1, double* m2) {
	double* ret = new double[16];
	_add_matrices(m1, m2, ret);
	return ret;
}

inline double* AsmWrapper::subtract_matrices(double* m1, double* m2) {
	double* ret = new double[16];
	_subtract_matrices(m1, m2, ret);
	return ret;
}

inline double* AsmWrapper::mul_matrix_by_vec(double* m, double* v) {
	double* ret = new double[4];
	double* tmp = new double[4];
	for (size_t i = 0; i < 4; i += 1) {
		_mul_vec_by_vecT(m + 4 * i, v, tmp);
		ret[i] = tmp[0];	// this is ugly :(
	}
	return ret;
}
