// nn_cpp.cpp : Defines the exported functions for the DLL.
#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include "nn_cpp.h"

#include <cstdlib>
#include <stdlib.h>

void multiply_matrix_by_vector_cpp(double* matrix, double* vector, double* ret)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ret[i] += (matrix[4*i+j] * vector[j]);
        }
    }
}

void multiply_matrix_by_constant_cpp(double* matrix, double constant, double* ret)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ret[4*i+j] = constant + matrix[4*i+j];
        }
    }
}

void multiply_vector_by_constant_cpp(double* vector, double constant, double* ret)
{
    for (int i = 0; i < 4; i++) {
        ret[i] = constant * vector[i];
    }
}

void add_matrices_cpp(double* A, double* B, double* ret)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ret[4*i+j] = A[4*i+j] + B[4*i+j];
        }
    }
}

void subtract_matrices_cpp(double* A, double* B, double* ret)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ret[4*i+j] = A[4*i+j] - B[4*i+j];
        }
    }
}

void subtract_constant_from_vector_cpp(double constant, double* A, double* ret)
{
    for (int i = 0; i < 4; i++) {
        ret[i] = A[i] - constant;
    }
}

void subtract_vectors_cpp(double* A, double* B, double* ret)
{
    for (int i = 0; i < 4; i++) {
        ret[i] = A[i] - B[i];
    }
}

void add_vectors_cpp(double* v1, double* v2, double* ret)
{
    for (int i = 0; i < 4; i++) {
        ret[i] = v1[i] + v2[i];
    }
}

void multiply_vectorT_by_vector_cpp(double* vecT, double* vec, double* ret)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ret[4*i+j] = vecT[i] * vec[j];
        }
    }
}

void multiply_vectorT_by_constant_cpp(double* vecT, double constant, double* ret)
{
    for (int i = 0; i < 4; i++) {
        ret[i] = vecT[i] * constant;
    }
}

void element_wise_multiply_cpp(double* vec1, double* vec2, double* ret)
{
    for (int i = 0; i < 4; i++) {
        ret[i] = vec1[i] * vec2[i];
    }
}

void relu_vector_cpp(double* x, double* ret)
{
    for (int i = 0; i < 4; i++) {
        if (x[i] >= 0) {
            ret[i] = x[i];
        }
        else {
            ret[i] = 0.0;
        }
    }
}

void derivative_relu_vector_cpp(double* x, double* _, double* ret)
{
    for (int i = 0; i < 4; i++) {
        if (x[i] < 0) {
            ret[i] = 0.0;
        }
        else {
            ret[i] = 1.0;
        }
    }
}



