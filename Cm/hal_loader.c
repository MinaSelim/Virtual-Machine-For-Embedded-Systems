#include "_stdtype.h"
#include "hal_loader.h"
#include "bsl_loader.h"

u8* hal_load_program(int argc, char** argv)
{
	return bsl_load_Program(argc, argv);
}

void hal_unload_program(void)
{
	bsl_unload_Program();
}