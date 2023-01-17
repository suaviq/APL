#pragma once
#include <windows.h>


extern "C" double* _stdcall subtract_vectors(DWORDLONG x, DWORDLONG y);
typedef int(__stdcall *f_subtract_vectors)(double*, double*);

extern "C" double* _stdcall add_vectors(DWORDLONG x, DWORDLONG y);
typedef int(__stdcall *f_add_vectors)(double*, double*);

extern "C" double* _stdcall add_scalar_to_vector(DWORDLONG x, DWORDLONG y);
typedef int(__stdcall *f_add_scalar_to_vector)(double*, double*);

extern "C" double* _stdcall subtract_scalar_from_vector(DWORDLONG x, DWORDLONG y);
typedef int(__stdcall *f_subtract_scalar_from_vector)(double*, double*);
