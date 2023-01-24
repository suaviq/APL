#pragma once
#include <windows.h>


typedef void(__stdcall* f_asm_f64x2)(double*, double*);
typedef void(__stdcall* f_asm_f64x3)(double*, double*, double*);

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