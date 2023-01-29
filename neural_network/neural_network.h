#pragma once
#include "layer.h"
#include <iostream>
#include <fstream>
using namespace std;

//neural network as a list of layers 

class NeuralNetwork
{
private:
	int n_layers;
	Layer* hidden_layers;
	OutputLayer output_layer;
public:
	NeuralNetwork(int n_layers, double learning_rate = 0.01)
	{

		Layer* hidden_layers = new Layer[n_layers];
		hidden_layers[0] = Layer(learning_rate);
		for (int l = 1; l < n_layers; l++)
		{
			hidden_layers[l] = Layer(learning_rate);
		}
		OutputLayer output_layer = OutputLayer(learning_rate);
		this->n_layers = n_layers;
		this->hidden_layers = hidden_layers;
		this->output_layer = output_layer;
	}

	void fit(double** X, double** y, int epochs, int m)
	{

		/*double* X = new double[16];
		for (size_t i = 0; i < 16; i++) {
			X[i] = X_raw[i];
		}*/
		/*m -> size of dataset/number of elements in dataset
		n -> size of one element in dataset, np X[0] = [1, 2, 3] --> n = 3*/

		for (int epoch = 0; epoch < epochs; epoch++)
		{
			for (int i = 0; i < m; i++)
			{
				cout << "\r" << "epoch: " << epoch << " progress: " << i/m * 100 << "%";
				// forward
				this->hidden_layers[0].forward(X[i]);
				for (int l = 1; l < this->n_layers; l++)
				{
					this->hidden_layers[l].forward(hidden_layers[l - 1].access_a());
				}
				this->output_layer.forward(hidden_layers[this->n_layers - 1].access_a());

				// backward
				this->output_layer.backward(y[i]);
				this->hidden_layers[this->n_layers - 1].backward(this->output_layer.access_W(), this->output_layer.access_b(), this->output_layer.access_delta());
				for (int l = this->n_layers - 2; l > -1; l--)
				{
					this->hidden_layers[l].backward(this->hidden_layers[l + 1].access_W(), this->hidden_layers[l + 1].access_b(), this->hidden_layers[l + 1].access_delta());
				}
				cout << endl;
			}
		}
	}


	double** predict(double** X)
	{
		double** predicted = new double*[4];
		for (int i = 0; i < 4; i++)
		{
			// forward
			this->hidden_layers[0].forward(X[i]);
			for (int l = 1; l < this->n_layers; l++)
			{
				this->hidden_layers[l].forward(hidden_layers[l - 1].access_a());
			}
			this->output_layer.forward(hidden_layers[this->n_layers - 1].access_a());

			// print results
			cout << "result for element " << i << ": ";
			predicted[i] = new double[4];
			for (int n = 0; n < 4; n++)
			{
				if (output_layer.access_a()[n] >= 0.5)
					predicted[i][n] = 1;
				else
					predicted[i][n] = 0;
				cout << predicted[i][n] << " ";
			}
			cout << endl;
		}
		return predicted;
	}
};


double** read_matrix_txt(string file_name)
{
	double** result = new double*[4];
	ifstream file;
	file.open(file_name);
	if (file.is_open())
	{
		for (int i = 0; i < 4; i++)
		{
			result[i] = new double[4];
			for (int j = 0; j < 4; j++)
			{
				file >> result[i][j];
				cout << result[i][j] << '\t';
			}
		}
	}
	else
	{
		cout << "Problem with opening the file" << endl;
	}
	file.close();
	return result;
}

