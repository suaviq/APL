#pragma once
#include "asm_wrapper.h"
#include "cpp_wrapper.h"
#include "dll_wrapper.h"
#include <string>
#include <ctime>


void cout_matrix(std::string name, double* M)
{
	std::cout << std::endl << "calculate " << name << ":" << std::endl;
	for (size_t i = 0; i < 4; i += 1) {
		printf("%x >\t", i);
		for (size_t j = 0; j < 4; j += 1) {
			printf("%4f  \t", M[4 * i + j]);
		}
		printf("\n");
	}
}

void cout_vector(std::string name, double* v)
{
	std::cout << std::endl << "calculate " << name << ":" << std::endl;
	for (size_t i = 0; i < 4; i += 1) {
		printf("%lf  ", v[i]);
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
		if (!a_prev) delete a_prev;
		delete delta;
		delete error;
		delete learning_rate;
	}

	Layer(double lr, int wrapper_choice) {
		W = initialize_layer_weights();
		b = initialize_layer_bias();
		z = new double[4]{ 0.0 };
		a = new double[4]{ 0.0 };
		a_prev = new double[4]{ 0.0 };
		delta = new double[4]{ 0.0 };
		error = new double[4]{ 0.0 };
		learning_rate = new double(lr);
		switch(wrapper_choice) {
		case 0:
			this->dll = (DllWrapper*) & aw;
			break;
		case 1:
			this->dll = (DllWrapper*) & cw;
			break;
		default:
			this->dll = (DllWrapper*) & aw;
			break;
		}

	}

	double* forward(double* a_prev, int verbose = 1) {
		
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
	srand(time(0));
	double* matrix = new double[16];
	for (size_t i = 0; i < 16; i += 1) {
		matrix[i] = double((rand() % 100)) / double(10000);
		//std::cout << matrix[i] << "\t";
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
	using Layer::forward;
	using Layer::Layer;

	void backward(double* y, int verbose = 1) {
		// update bias
		
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
};


void test_layer(const int layers_count, const int dll_type) {
	const int m = 4;
	const int epochs = 1;
	int verbose = 1; //bigger than 1 to print things
	double learning_rate = 0.5;
	// linear function y = 2x+1
	double x_raw[m][4] = { 1.0, 2.0, 1.5, 3.3, 
						1.0, 2.0, 1.5, 3.3,
						2.0, 6.1, 9.5, 6.3,
						2.0, 6.1, 9.5, 6.3 };

	double y_raw[m][4] = { 3.0, 5.0, 4.0, 7.6,
						3.0, 5.0, 4.0, 7.6,
						5.0, 13.2, 20.0, 13.6,
						5.0, 13.2, 20.0, 13.6 };
	double** x = new double* [m];
	double** y = new double* [m];

	for (int i = 0; i < m; i++) {
		x[i] = new double[4];
		y[i] = new double[4];
		for (int j = 0; j < 4; j++) {
			x[i][j] = x_raw[i][j];
			y[i][j] = y_raw[i][j];
		}
	}

	Layer hidden_layer_1 = Layer(learning_rate, dll_type);
	Layer hidden_layer_2 = Layer(learning_rate, dll_type);
	Layer hidden_layer_3 = Layer(learning_rate, dll_type);
	OutputLayer out_layer = OutputLayer(learning_rate, dll_type);
	
	std::cout << "---------------------------------  FIT  ------------------------------------" << std::endl;
	for (int epoch = 0; epoch < epochs; ++epoch)
	{
		for (int i = 0; i < m; ++i)
		{
			double loss = 0;
			hidden_layer_1.forward(x[i], verbose);
			hidden_layer_2.forward(hidden_layer_1.access_a(), verbose);
			hidden_layer_3.forward(hidden_layer_2.access_a(), verbose);
			out_layer.forward(hidden_layer_3.access_a(), verbose);

			out_layer.backward(y[i], verbose);
			hidden_layer_3.backward(out_layer.access_W(), out_layer.access_b(), out_layer.access_delta(), verbose);
			hidden_layer_2.backward(hidden_layer_3.access_W(), hidden_layer_3.access_b(), hidden_layer_3.access_delta(), verbose);
			hidden_layer_1.backward(hidden_layer_2.access_W(), hidden_layer_2.access_b(), hidden_layer_2.access_delta(), verbose);

			loss += get_loss(out_layer.access_delta())/(i+1);

			std::cout << "epoch: " << epoch + 1 << "\tprogress: " << double(i + 1) / m * 100 << "%" << "\tloss: " << loss << std::endl;
		}
	}

	std::cout << "---------------------------------  PREDICT  --------------------------------" << std::endl;

	for (int i = 0; i < m; ++i)
	{
		hidden_layer_1.forward(x[i], verbose);
		hidden_layer_2.forward(hidden_layer_1.access_a(), verbose);
		hidden_layer_3.forward(hidden_layer_2.access_a(), verbose);
		out_layer.forward(hidden_layer_3.access_a(), verbose);
		cout_vector("y true", y[i]);
		cout_vector("prediction", out_layer.access_a());
	}

	delete[] x;
	delete[] y;
}
