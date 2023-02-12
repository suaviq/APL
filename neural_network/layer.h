#pragma once
#include "asm_wrapper.h"
#include "cpp_wrapper.h"
#include "dll_wrapper.h"

#include <string>
#include <random>

void cout_matrix(std::string name, double* M)
{
	std::cout << std::endl << "calculate " << name << ":" << std::endl;
	for (size_t i = 0; i < 4; i += 1) {
		printf("%x >\t", i);
		for (size_t j = 0; j < 4; j += 1) {
			printf("%04lf  \t", M[4 * i + j]);
		}
		printf("\n");
	}
}

void cout_vector(std::string name, double* v)
{
	std::cout << std::endl << "calculate " << name << ":" << std::endl;
	for (size_t i = 0; i < 4; i += 1) {
		printf("%04lf  ", v[i]);
	}
	printf("\n");
}

double get_loss(double *delta)
{
	double loss = 0.0;
	for (int i = 0; i < 4; i++)
	{
		loss += abs(delta[i]);
	}
	return loss;
}


class Layer {
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
	CppWrapper cw = CppWrapper();

	DllWrapper* dll;

	double* initialize_layer_weights();
	double* initialize_layer_bias();
	double* transpoze(double* m);

public:
	Layer() = default;
	~Layer() {
		delete W;
		delete b;
		delete z;
		delete a;
		delete delta;
		delete error;
	}

	Layer(double lr, int wrapper_choice) {
		W = initialize_layer_weights();
		b = initialize_layer_bias();
		z = new double[4]{ 0.0 };
		a = new double[4]{ 0.0 };
		a_prev = new double[4]{ 0.0 };
		delta = new double[4]{ 0.0 };
		error = new double[4]{ 0.0 };
		this->learning_rate = new double(lr);
		this->aw = AsmWrapper();
		this->cw = CppWrapper();
		switch(wrapper_choice) {
		case 0:
			this->dll = (DllWrapper*) &this->aw;
			break;
		case 1:
			this->dll = (DllWrapper*) &this->cw;
			break;
		default:
			this->dll = (DllWrapper*) &this->aw;
			break;
		}

	}

	double* forward(double* a_prev, int verbose = 1) {
		// z -> a_prev * W + b
		// a -> activation(z)
		this->z = dll->mul_matrix_by_vec(this->W, a_prev);
		this->a = dll->relu_vec(this->z);
		this->a_prev = a_prev;

		if (verbose > 1)
		{
			std::cout << std::endl << "FORWARD PROPAGATION HIDDEN LAYER:" << std::endl;
			cout_matrix("W", W);
			cout_vector("z", z);
			cout_vector("a", a);
			cout_vector("a_prev", a_prev);
		}
		return this->a;
	}

	void backward(double* W_next, double* b_next, double* delta_next, int verbose = 1) {
		// error = W_next * delta_next + b_next * delta_next
		// delta -> error . activation_derivative(a) . -> element wise multiplications [1, 2, 3] . [2, 2, 2] = [1*2, 2*2, 3*2]
		// W -> W - learning_rate * a_prev.T * delta  -> [1, 2].T * [1, 2] = matrix
		// b -> b - learning_rate * delta
		//std::cout << std::endl << "BACKWARD PROPAGATION:" << std::endl;
		// weight
		double* W_nextT = transpoze(W_next);
		this->error = dll->mul_matrix_by_vec(W_nextT, delta_next);
		// delta
		this->delta = dll->vector_wise_multiply(this->error, dll->derivative_relu_vec(this->a));

		// update weights
		double* w = dll->mul_vecT_by_vec(this->delta, this->a_prev);
		double* w1 = dll->mul_matrix_by_scalar(w, this->learning_rate);
		this->W = dll->subtract_matrices(this->W, w1);

		// update bias
		this->b = dll->subtract_vectors(this->b, dll->vector_wise_multiply(this->b, this->delta));

		if (verbose > 1)
		{
			std::cout << std::endl << "BACKWARD PROPAGATION HIDDEN LAYER:" << std::endl;
			cout_vector("a", a);
			cout_vector("error", error);
			cout_vector("delta", delta);
			cout_vector("delta_next", delta_next);
			cout_matrix("w", w);
			cout_matrix("w1", w1);
			cout_matrix("W", W);
			cout_vector("bias", b);
		}
		
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

double* Layer::initialize_layer_weights() {
	std::random_device rd;
	std::mt19937 gen(rd());
	double* matrix = new double[16];
	double low = 0;
	for (size_t i = 0; i < 16; i += 1) {
		low = int(i / 4) * 0.5;
		std::uniform_real_distribution<> dis(low, low + 0.5);
		matrix[i] = dis(gen) / 4000;
	}
	return matrix;
}

double* Layer::initialize_layer_bias() {
	srand(time(0));
	double* vec = new double[4];
	for (size_t i = 0; i < 4; i += 1) {
		vec[i] = double((rand() % 100)) / double(10000);
	}
	return vec;
}

double* Layer::transpoze(double* m) {
	double* matrixT = new double[16];
	for (size_t i = 0; i < 4; i += 1) {
		for (size_t j = 0; j < 4; j += 1) {
			matrixT[4 * i + j] = m[4 * j + i];
		}
	}
	return matrixT;
}

class OutputLayer : public Layer {
public:
	void backward(double* y, int verbose = 1) {
		// error -> a - y									
		// delta -> error_w* activation_derivative(a)	
		
		this->error = dll->subtract_vectors(this->a, y);
		// weight
		this->delta = dll->vector_wise_multiply(this->error, dll->derivative_relu_vec(this->a));
		// update weights
		double* w = dll->mul_vecT_by_vec(this->delta, this->a_prev);
		double* w1 = dll->mul_matrix_by_scalar(w, this->learning_rate);
		this->W = dll->subtract_matrices(this->W, w1);
		// update bias
		this->b = dll->subtract_vectors(this->b, dll->vector_wise_multiply(this->b, this->delta));
		
		if (verbose > 1)
		{
			std::cout << std::endl << "BACKWARD PROPAGATION OUTPUT LAYER:" << std::endl;
			cout_vector("a", a);
			cout_vector("y", y);
			cout_vector("error", error);
			cout_vector("delta", delta);
			cout_matrix("w", w);
			cout_matrix("w1", w1);
			cout_matrix("W", W);
			cout_vector("bias", b);
		}
		// return this->b;
	}
	OutputLayer(double lr, int wrapper_choice) {
		W = initialize_layer_weights();
		b = initialize_layer_bias();
		z = new double[4] { 0.0 };
		a = new double[4] { 0.0 };
		a_prev = new double[4] { 0.0 };
		delta = new double[4] { 0.0 };
		error = new double[4] { 0.0 };
		this->learning_rate = new double(lr);
		this->aw = AsmWrapper();
		this->cw = CppWrapper();
		switch (wrapper_choice) {
		case 0:
			this->dll = (DllWrapper*)&this->aw;
			break;
		case 1:
			this->dll = (DllWrapper*)&this->cw;
			break;
		default:
			this->dll = (DllWrapper*)&this->aw;
			break;
		}
	}
};


void run_network(const unsigned int layers_count, const unsigned int epochs, const unsigned int dll_type, const int verbose) {
	const int m = 16;
	double learning_rate = 0.5;

	// linear function y = 2x+1
	double x_raw[m][4] = {
		{ 1.0, 2.0, 1.5, 3.3 },
		{ 1.0, 2.0, 1.5, 3.3 },
		{ 2.0, 6.1, 9.5, 6.3 },
		{ 2.0, 6.1, 9.5, 6.3 },
		{ 1.5, 2.5, 3.5, 4.5 },
		{ 1.5, 2.5, 3.5, 4.5 },
		{ 9.5, 8.5, 3.3, 1.9 },
		{ 5.5, 6.6, 2.3, 1.3 },

		{ 1.0, 2.0, 1.5, 3.3 },
		{ 1.0, 2.0, 1.5, 3.3 },
		{ 2.0, 6.1, 9.5, 6.3 },
		{ 2.0, 6.1, 9.5, 6.3 },
		{ 1.5, 2.5, 3.5, 4.5 },
		{ 1.5, 2.5, 3.5, 4.5 },
		{ 9.5, 8.5, 3.3, 1.9 },
		{ 5.5, 6.6, 2.3, 1.3 },
	};
	double y_raw[m][4] = { 0.0 };

	double** x = new double* [m];
	double** y = new double* [m];

	for (int i = 0; i < m; i++) {
		x[i] = new double[4];
		y[i] = new double[4];
		for (int j = 0; j < 4; j++) {
			x[i][j] = x_raw[i][j];
			y[i][j] = x_raw[i][j] * 2 + 1;
			// previously:
			// y[i][j] = y_raw[i][j];
		}
	}

	Layer* hidden_layers = new Layer[layers_count];
	for (size_t i = 0; i < layers_count; ++i) {
		hidden_layers[i] = Layer(learning_rate, dll_type);
	}
	OutputLayer out_layer = OutputLayer(learning_rate, dll_type);
	
	if (verbose >= 0) std::cout << "---------------------------------  FIT  ------------------------------------" << std::endl;
	for (unsigned int epoch = 0; epoch < epochs; ++epoch)
	{
		if (verbose >= 0) printf(
			"> epoch: %d\n",
			epoch+1
		);
		for (int i = 0; i < m; ++i)
		{
			double loss = 0;
			hidden_layers[0].forward(x[i], verbose);
			for (size_t i = 1; i < layers_count; ++i) {
				hidden_layers[i].forward(hidden_layers[i-1].access_a(), verbose);
			}
			out_layer.forward(hidden_layers[layers_count-1].access_a(), verbose);

			out_layer.backward(y[i], verbose);
			hidden_layers[layers_count-1].backward(out_layer.access_W(), out_layer.access_b(), out_layer.access_delta(), verbose);
			for (size_t i = layers_count - 2; i > 0; --i) {
				hidden_layers[i].backward(
					hidden_layers[i + 1].access_W(),
					hidden_layers[i + 1].access_b(),
					hidden_layers[i + 1].access_delta(),
					verbose
				);
			}
			hidden_layers[0].backward(
				hidden_layers[1].access_W(), 
				hidden_layers[1].access_b(), 
				hidden_layers[1].access_delta(), 
				verbose
			);

			loss += get_loss(out_layer.access_delta())/(i+1);

			if (verbose >= 0) printf(
				"  > progress: % .1lf % %\n  > loss: % .1lf\n\n",
				double(i+1)/m*100, loss
			);
		}
	}

	if (verbose >= 0) std::cout << "---------------------------------  PREDICT  --------------------------------" << std::endl;

	for (int i = 0; i < m; ++i)
	{
		hidden_layers[0].forward(x[i], verbose);
		for (size_t i = 1; i < layers_count; ++i) {
			hidden_layers[i].forward(hidden_layers[i-1].access_a(), verbose);
		}
		out_layer.forward(hidden_layers[layers_count-1].access_a(), verbose);
		if (verbose >= 0) {
			cout_vector("y true", y[i]);
			cout_vector("prediction", out_layer.access_a());
		}
	}

	// delete[] hidden_layers;
	delete[] x;
	delete[] y;
}
