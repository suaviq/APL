#pragma once
#include "asm_wrapper.h"
#include <string>
#include <ctime>

class AsmLayer {
protected:
    double* W;  // weights
    double* b;  // bias
    double* z;  // neuron value
    double* a;  // neuron value after activation
    double* a_prev; // previous neuron value after activation
    double* delta;  // delta
    double* error;  // error
    double* learning_rate;
    AsmWrapper aw = AsmWrapper();

    double* initialize_layer_weights();
    double* initialize_layer_bias();
    double* transpoze(double* m);

public:
    AsmLayer() = default;
    ~AsmLayer() {
        delete W;
        delete b;
        delete z;
        delete a;
        delete a_prev;
        delete delta;
        delete error;
        delete learning_rate;
    }

    AsmLayer(double lr) {
        W = initialize_layer_weights();
        b = initialize_layer_bias();
        z = new double[4]  { 0.0 };
        a = new double[4]  { 0.0 };
        a_prev = new double[4]  { 0.0 };
        delta  = new double[16] { 0.0 };
        error  = new double[16] { 0.0 };
        *learning_rate = lr;
    }

    double* forward(double* a_prev) {
        this->z = aw.mul_matrix_by_vec(this->W, a_prev);
        this->a = aw.relu_vec(this->z);
        this->a_prev = a_prev;
        return this->a;
    }

    void backward(double* W_next, double* delta_next) {
        // weight
        double* W_nextT = transpoze(W_next);
        this->error = aw.mul_matrix_by_vec(W_nextT, delta_next);

        // delta
        this->delta = aw.vector_wise_multiply(this->error, aw.derivative_relu_vec(this->a));

        // update weights
        double* w = aw.mul_vecT_by_vec(this->delta, this->a_prev);
        double* w1 = aw.mul_matrix_by_scalar(w, this->learning_rate);
        this->W = aw.subtract_matrices(this->W, w1);

        // update bias
        this->b = aw.subtract_vectors(this->b, aw.vector_wise_multiply(this->b, this->delta));
    }

    double* access_a() {
        return this->a;
    }

    double* access_delta() {
        return this->delta;
    }

    double* access_b() {
        return this->b;
    }

    double* access_W() {
        return this->W;
    }
};

double* AsmLayer::initialize_layer_weights() {
    srand(time(0));
    double* matrix = new double[16];
    for (size_t i = 0; i < 16; i += 1) {
        matrix[i] = double((rand() % 100) - 50) / double(1000);
    }
    return matrix;
}

double* AsmLayer::initialize_layer_bias() {
    srand(time(0));
    double* vec = new double[4];
    for (size_t i = 0; i < 4; i += 1) {
        vec[i] = double((rand() % 100) - 50) / double(1000);
    }
    return vec;
}

double* AsmLayer::transpoze(double* m) {
    double* matrixT = new double[16];
    for (size_t i = 0; i < 4; i += 1) {
        for (size_t j = 0; j < 4; j += 1) {
            matrixT[4*i+j] = m[4*j+i];
        }
    }
    return matrixT;
}

class OutputLayer : public AsmLayer {
public:
    using AsmLayer::forward;
    using AsmLayer::AsmLayer;

    void backward(double* y) {
        // weight
        this->error = aw.subtract_vectors(this->a, y);
        this->delta = aw.vector_wise_multiply(this->error, aw.derivative_relu_vec(this->a));

        // update weights
        double* w = aw.mul_vecT_by_vec(this->delta, this->a_prev);
        double* w1 = aw.mul_matrix_by_scalar(w, this->learning_rate);
        this->W = aw.subtract_matrices(this->W, w1);

        // update bias
        this->b = aw.subtract_vectors(this->b, aw.vector_wise_multiply(this->b, this->delta));
    }
};

void test_layer() {
    double x_raw[4] = { 1.0, 2.0, 1.5, 3.3 };
    double y_raw[4] = { 0.5, 1.3, 4.2, 2.8 };

    double* x = new double[4];
    double* y = new double[4];

    for (size_t i = 0; i < 4; i++) {
        x[i] = x_raw[i];
        y[i] = y_raw[i];
    }

    AsmLayer hidden_layer = AsmLayer(0.01);
    OutputLayer out_layer = OutputLayer(0.01);

    hidden_layer.forward(x);
    out_layer.forward(hidden_layer.access_a());

    out_layer.backward(y);
    hidden_layer.backward(out_layer.access_W(), out_layer.access_delta());

    delete[] x;
    delete[] y;
}
