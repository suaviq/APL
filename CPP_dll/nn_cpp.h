// nn_cpp.h - Contains declarations of math functions
#pragma once

#define nn_cpp_EXPORTS

#ifdef nn_cpp_EXPORTS
#define nn_cpp __declspec(dllexport)
#else
#define nn_cpp __declspec(dllimport)
#endif

extern "C" nn_cpp 
void multiply_matrix_by_vector_cpp(double* matrix, double* vector, double* ret);

extern "C" nn_cpp 
void multiply_matrix_by_constant_cpp(double* matrix, double* constant, double* ret);

extern "C" nn_cpp
void multiply_vector_by_constant_cpp(double* vector, double* constant, double* ret);

extern "C" nn_cpp
void add_matrices_cpp(double* A, double* B, double* ret);

extern "C" nn_cpp
void subtract_matrices_cpp(double* A, double* B, double* ret);

extern "C" nn_cpp
void subtract_constant_from_vector_cpp(double* constant, double* A, double* ret);

extern "C" nn_cpp
void subtract_vectors_cpp(double* A, double* B, double* ret);

extern "C" nn_cpp
void add_vectors_cpp(double* v1, double* v2, double* ret);

extern "C" nn_cpp
void multiply_vectorT_by_vector_cpp(double* vecT, double* vec, double* ret);

extern "C" nn_cpp
void multiply_vectorT_by_constant_cpp(double* vecT, double* constant, double* ret);

extern "C" nn_cpp
void element_wise_multiply_cpp(double* vec1, double* vec2, double* ret);

extern "C" nn_cpp
void relu_vector_cpp(double* x, double* ret);

extern "C" nn_cpp
void derivative_relu_vector_cpp(double* x, double* _, double* ret);
