#include <windows.h>
#include <iostream>
#include <string>

#include "paths.h"
#include "asm_wrapper.h"
#include "layer.h"

enum DllType {
	ASM = 0,
	CPP = 1
};

int main()
{
	test_layer(5, DllType::ASM);
	return EXIT_SUCCESS;
}
