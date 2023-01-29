#pragma once

#include <windows.h>
#include <iostream>
#include "paths.h"

// Function Types
typedef void(__stdcall* f_dll_f64x2)(double*, double*);
typedef void(__stdcall* f_dll_f64x3)(double*, double*, double*);

class DllWrapper {
private:
public:
	virtual inline double* add_vectors(double* v1, double* v2) = 0;
	virtual inline double* subtract_vectors(double* v1, double* v2) = 0;
	virtual inline double* add_scalar_to_vector(double* v, double* s) = 0;
	virtual inline double* subtract_scalar_from_vector(double* v, double* s) = 0;
	virtual inline double* vector_wise_multiply(double* v1, double* v2) = 0;
	virtual inline double* mul_vecT_by_vec(double* vT, double* v) = 0;
	virtual inline double  mul_vec_by_vecT(double* v, double* vT) = 0;
	virtual inline double* relu_vec(double* v) = 0;
	virtual inline double* derivative_relu_vec(double* v) = 0;
	virtual inline double* mul_vector_by_scalar(double* v, double* s) = 0;
	virtual inline double* mul_matrix_by_scalar(double* m, double* s) = 0;
	virtual inline double* add_matrices(double* m1, double* m2) = 0;
	virtual inline double* subtract_matrices(double* m1, double* m2) = 0;
	virtual inline double* mul_matrix_by_vec(double* m, double* v) = 0;

	DllWrapper() = default;
	~DllWrapper() = default;
};
