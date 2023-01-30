#include <windows.h>
#include <iostream>
#include <string>
#include <chrono>

#include "paths.h"
#include "asm_wrapper.h"
#include "layer.h"

enum DllType {
	ASM = 0,
	CPP = 1
};

int main()
{
	using namespace std::chrono;

	std::string dll_type;
	unsigned int layers_count, epochs_count;
	int verbosity;

	std::cout << "Provide number of layers, number of epochs, type of DLL [asm|cpp], and level of verbosity [-1|1|2]\n > ";
	std::cin >> layers_count >> epochs_count >> dll_type >> verbosity;
	std::cout 
		<< "layers_count: " << layers_count << '\n'
		<< "epochs_count: " << epochs_count << '\n'
		<< "dll_type:     " << dll_type << '\n'
		<< "verbosity:    " << verbosity << '\n';

	auto start = high_resolution_clock::now();
	test_layer(
		layers_count,
		epochs_count,
		dll_type == "asm" ? DllType::ASM : DllType::CPP,
		verbosity
	);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	std::cout << "\n[INFO] Program took " << duration.count() << " microseconds\n";

	return EXIT_SUCCESS;
}
