#pragma once

#include "dll_wrapper.h"


extern "C" double* _stdcall _add_vectors_cpp(DWORDLONG x, DWORDLONG y, DWORDLONG result);
extern "C" double* _stdcall _subtract_vectors_cpp(DWORDLONG x, DWORDLONG y, DWORDLONG result);
extern "C" double* _stdcall _add_scalar_to_vector_cpp(DWORDLONG x, DWORDLONG y, DWORDLONG result);
extern "C" double* _stdcall _subtract_scalar_from_vector_cpp(DWORDLONG x, DWORDLONG y, DWORDLONG result);
extern "C" double* _stdcall _vector_wise_multiply_cpp(DWORDLONG x, DWORDLONG y, DWORDLONG result);
extern "C" double* _stdcall _mul_vecT_by_vec_cpp(DWORDLONG x, DWORDLONG y, DWORDLONG result);
extern "C" double* _stdcall _mul_vec_by_vecT_cpp(DWORDLONG x, DWORDLONG y, DWORDLONG result);
extern "C" double* _stdcall _relu_vec_cpp(DWORDLONG x, DWORDLONG result);
extern "C" double* _stdcall _derivative_relu_vec_cpp(DWORDLONG x, DWORDLONG y, DWORDLONG result);
extern "C" double* _stdcall _mul_vector_by_scalar_cpp(DWORDLONG x, DWORDLONG y, DWORDLONG result);
extern "C" double* _stdcall _mul_matrix_by_scalar_cpp(DWORDLONG x, DWORDLONG y, DWORDLONG result);
extern "C" double* _stdcall _add_matrices_cpp(DWORDLONG x, DWORDLONG y, DWORDLONG result);
extern "C" double* _stdcall _subtract_matrices_cpp(DWORDLONG x, DWORDLONG y, DWORDLONG result);


/*

	TODO: 
	  1. Do something about functions that do not have representation in DLL

*/

class CppWrapper : private DllWrapper {

private:
	// DLL Instance
	HINSTANCE hGetProcIDDLL;
	// DLL Procedures
	f_dll_f64x3 _add_vectors;
	f_dll_f64x3 _subtract_vectors;
	f_dll_f64x3 _add_scalar_to_vector;
	f_dll_f64x3 _subtract_scalar_from_vector;
	f_dll_f64x3 _vector_wise_multiply;
	f_dll_f64x3 _mul_vecT_by_vec;
	f_dll_f64x3 _mul_vec_by_vecT;
	f_dll_f64x2 _relu_vec;
	f_dll_f64x3 _derivative_relu_vec;
	f_dll_f64x3 _mul_vector_by_scalar;
	f_dll_f64x3 _mul_matrix_by_scalar;
	f_dll_f64x3 _mul_matrix_by_vec;
	f_dll_f64x3 _add_matrices;
	f_dll_f64x3 _subtract_matrices;

public:
	// Accessible functions
	inline double* add_vectors(double* v1, double* v2) override;
	inline double* subtract_vectors(double* v1, double* v2) override;
	inline double* add_scalar_to_vector(double* v, double* s) override;
	inline double* subtract_scalar_from_vector(double* v, double* s) override;
	inline double* vector_wise_multiply(double* v1, double* v2) override;
	inline double* mul_vecT_by_vec(double* vT, double* v) override;
	inline double  mul_vec_by_vecT(double* v, double* vT) override { double x = double(0); return x; }
	inline double* relu_vec(double* v) override;
	inline double* derivative_relu_vec(double* v) override;
	inline double* mul_vector_by_scalar(double* v, double* s) override;
	inline double* mul_matrix_by_scalar(double* m, double* s) override;
	inline double* add_matrices(double* m1, double* m2) override;
	inline double* subtract_matrices(double* m1, double* m2) override;
	inline double* mul_matrix_by_vec(double* m, double* v) override;

	// Constructor & Destructor
	CppWrapper();
	~CppWrapper() = default;

};

CppWrapper::CppWrapper() {
#if _DEBUG
	std::cout << "Loading Debug C++ DLL...\n";
	HINSTANCE hGetProcIDDLL = LoadLibrary(DEBUG_CPP_DLL_PATH);
#else
	std::cout << "Loading Release C++ DLL...\n";
	HINSTANCE hGetProcIDDLL = LoadLibrary(CPP_DLL_PATH);
#endif
	
	// Check if DLL Exists
	if (!hGetProcIDDLL) {
		std::cerr << "[ERROR] Could not load the dynamic library\n(Check if the path to the dll is correct)\n";
		exit( EXIT_FAILURE );
	}

	// Assign procedures from DLL
	_subtract_vectors = (f_dll_f64x3)GetProcAddress(hGetProcIDDLL, "subtract_vectors_cpp");
	if (!_subtract_vectors) {
		std::cerr << "[ERROR] could not locate the function `subtract_vectors_cpp`" << std::endl;
		exit( EXIT_FAILURE );
	}
	_add_vectors = (f_dll_f64x3)GetProcAddress(hGetProcIDDLL, "add_vectors_cpp");
	if (!_add_vectors) {
		std::cerr << "[ERROR] could not locate the function `add_vectors_cpp`" << std::endl;
		exit( EXIT_FAILURE );
	}
	// _add_scalar_to_vector = (f_dll_f64x3)GetProcAddress(hGetProcIDDLL, "add_scalar_to_vector_cpp");
	// if (!_add_scalar_to_vector) {
	// 	std::cerr << "[ERROR] could not locate the function `add_scalar_to_vector_cpp`" << std::endl;
	// 	exit( EXIT_FAILURE );
	// }
	// _subtract_scalar_from_vector = (f_dll_f64x3)GetProcAddress(hGetProcIDDLL, "subtract_scalar_from_vector_cpp");
	// if (!_subtract_scalar_from_vector) {
	// 	std::cerr << "[ERROR] could not locate the function `subtract_scalar_from_vector_cpp`" << std::endl;
	// 	exit( EXIT_FAILURE );
	// }
	_mul_vector_by_scalar = (f_dll_f64x3)GetProcAddress(hGetProcIDDLL, "multiply_vector_by_constant_cpp");
	if (!_mul_vector_by_scalar) {
		std::cerr << "[ERROR] could not locate the function `mul_vector_by_scalar_cpp`" << std::endl;
		exit( EXIT_FAILURE );
	}
	_vector_wise_multiply = (f_dll_f64x3)GetProcAddress(hGetProcIDDLL, "element_wise_multiply_cpp");
	if (!_vector_wise_multiply) {
		std::cerr << "[ERROR] could not locate the function `vector_wise_multiply_cpp`" << std::endl;
		exit( EXIT_FAILURE );
	}
	_mul_vecT_by_vec = (f_dll_f64x3)GetProcAddress(hGetProcIDDLL, "multiply_vectorT_by_vector_cpp");
	if (!_mul_vecT_by_vec) {
		std::cerr << "[ERROR] could not locate the function `mul_vecT_by_vec_cpp`" << std::endl;
		exit( EXIT_FAILURE );
	}
	// _mul_vec_by_vecT = (f_dll_f64x3)GetProcAddress(hGetProcIDDLL, "mul_vec_by_vecT_cpp");
	// if (!_mul_vec_by_vecT) {
	// 	std::cerr << "[ERROR] could not locate the function `mul_vec_by_vecT_cpp`" << std::endl;
	// 	exit( EXIT_FAILURE );
	// }
	_relu_vec = (f_dll_f64x2)GetProcAddress(hGetProcIDDLL, "relu_vector_cpp");
	if (!_relu_vec) {
		std::cerr << "[ERROR] could not locate the function `relu_vec_cpp`" << std::endl;
		exit( EXIT_FAILURE );
	}
	_derivative_relu_vec = (f_dll_f64x3)GetProcAddress(hGetProcIDDLL, "derivative_relu_vector_cpp");
	if (!_derivative_relu_vec) {
		std::cerr << "[ERROR] could not locate the function `derivative_relu_vec_cpp`" << std::endl;
		exit( EXIT_FAILURE );
	}
	_add_matrices = (f_dll_f64x3)GetProcAddress(hGetProcIDDLL, "add_matrices_cpp");
	if (!_add_matrices) {
		std::cerr << "[ERROR] could not locate the function `add_matrices_cpp`" << std::endl;
		exit( EXIT_FAILURE );
	}
	_subtract_matrices = (f_dll_f64x3)GetProcAddress(hGetProcIDDLL, "subtract_matrices_cpp");
	if (!_subtract_matrices) {
		std::cerr << "[ERROR] could not locate the function `subtract_matrices_cpp`" << std::endl;
		exit( EXIT_FAILURE );
	}
	_mul_matrix_by_scalar = (f_dll_f64x3)GetProcAddress(hGetProcIDDLL, "multiply_matrix_by_constant_cpp");
	if (!_mul_matrix_by_scalar) {
		std::cerr << "[ERROR] could not locate the function `mul_matrix_by_scalar_cpp`" << std::endl;
		exit( EXIT_FAILURE );
	}
	_mul_matrix_by_vec = (f_dll_f64x3)GetProcAddress(hGetProcIDDLL, "multiply_matrix_by_vector_cpp");
	if (!_mul_matrix_by_scalar) {
		std::cerr << "[ERROR] could not locate the function `mul_matrix_by_scalar_cpp`" << std::endl;
		exit( EXIT_FAILURE );
	}

	std::cout << "DLL Loaded succesfully!\n";
}


inline double* CppWrapper::add_vectors(double* v1, double* v2) {
	double* ret = new double[4] { 0.0 };
	_add_vectors(v1, v2, ret);
	return ret;
}

inline double* CppWrapper::subtract_vectors(double* v1, double* v2) {
	double* ret = new double[4] { 0.0 };
	_subtract_vectors(v1, v2, ret);
	return ret;
}

inline double* CppWrapper::add_scalar_to_vector(double* v, double* s) {
	double* ret = new double[4] { 0.0 };
	// _add_scalar_to_vector(s, v, ret);
	return ret;
}

inline double* CppWrapper::subtract_scalar_from_vector(double* v, double* s) {
	double* ret = new double[4] { 0.0 };
	_subtract_scalar_from_vector(s, v, ret);
	return ret;
}

inline double* CppWrapper::vector_wise_multiply(double* v1, double* v2) {
	double* ret = new double[4] { 0.0 };
	_vector_wise_multiply(v1, v2, ret);
	return ret;
}

inline double* CppWrapper::mul_vecT_by_vec(double* vT, double* v) {
	double* ret = new double[16] { 0.0 };
	_mul_vecT_by_vec(vT, v, ret);
	return ret;
}

inline double* CppWrapper::relu_vec(double* v) {
	double* ret = new double[4] { 0.0 };
	_relu_vec(v, ret);
	return ret;
}

inline double* CppWrapper::derivative_relu_vec(double* v) {
	double* ret = new double[4] { 0.0 };
	_derivative_relu_vec(v, new double[4] {1.0, 1.0, 1.0, 1.0}, ret);
	return ret;
}

inline double* CppWrapper::mul_vector_by_scalar(double* v, double* s) {
	double* ret = new double[4] { 0.0 };
	_mul_vector_by_scalar(v, s, ret);
	return ret;
}

inline double* CppWrapper::mul_matrix_by_scalar(double* m, double* s) {
	double* ret = new double[16] { 0.0 };
	_mul_matrix_by_scalar(m, s, ret);
	return ret;
}

inline double* CppWrapper::add_matrices(double* m1, double* m2) {
	double* ret = new double[16] { 0.0 };
	_add_matrices(m1, m2, ret);
	return ret;
}

inline double* CppWrapper::subtract_matrices(double* m1, double* m2) {
	double* ret = new double[16] { 0.0 };
	_subtract_matrices(m1, m2, ret);
	return ret;
}

inline double* CppWrapper::mul_matrix_by_vec(double* m, double* v) {
	double* ret = new double[4] { 0.0 };
	_mul_matrix_by_vec(m, v, ret);
	return ret;
}
