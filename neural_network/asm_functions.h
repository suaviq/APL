#pragma once
#include <windows.h>


extern "C" double* _stdcall add_vectors(DWORDLONG x, DWORDLONG y);
typedef int(__stdcall *f_add_vectors)(double*, double*);

extern "C" double* _stdcall subtract_vectors(DWORDLONG x, DWORDLONG y);
typedef int(__stdcall *f_subtract_vectors)(double*, double*);

extern "C" double* _stdcall add_scalar_to_vector(DWORDLONG x, DWORDLONG y);
typedef int(__stdcall *f_add_scalar_to_vector)(double*, double*);

extern "C" double* _stdcall subtract_scalar_from_vector(DWORDLONG x, DWORDLONG y);
typedef int(__stdcall *f_subtract_scalar_from_vector)(double*, double*);

extern "C" double* _stdcall vector_wise_multiply(DWORDLONG x, DWORDLONG y);
typedef int(__stdcall* f_vector_wise_multiply)(double*, double*);

extern "C" double* _stdcall mul_vecT_by_vec(DWORDLONG x, DWORDLONG y, DWORDLONG res);

extern "C" double* _stdcall mul_vector_by_scalar(DWORDLONG x, DWORDLONG y);
typedef int(__stdcall* f_mul_vector_by_scalar)(double*, double*);

extern "C" double* _stdcall mul_matrix_by_scalar(DWORDLONG x, DWORDLONG y);
typedef int(__stdcall* f_mul_matrix_by_scalar)(double*, double*);

extern "C" double* _stdcall add_matrices(DWORDLONG x, DWORDLONG y);
typedef int(__stdcall* f_add_matrices)(double*, double*);

extern "C" double* _stdcall subtract_matrices(DWORDLONG x, DWORDLONG y);
typedef int(__stdcall* f_subtract_matrices)(double*, double*);

typedef double(__stdcall* f_asm)(double*, double*);
typedef double(__stdcall* f_asm_3d)(double*, double*, double*);