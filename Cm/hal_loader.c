#include "_stdtype.h"
#include "hal_loader.h"
#include "bsl_loader.h"

// Loads and returns the program to be run by the VM
u8* hal_load_program(int argc, char** argv)
{
	return bsl_load_Program(argc, argv);
}

// Unloads the program from memory
void hal_unload_program(void)
{
	bsl_unload_Program();
}