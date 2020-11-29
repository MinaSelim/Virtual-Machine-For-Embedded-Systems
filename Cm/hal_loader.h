#ifndef HAL_LOADER_H
#define HAL_LOADER_H
#include "_stdtype.h"

u8* hal_load_program(int argc, char** argv);
void hal_unload_program(void);

#endif // !HAL_LOADER_H
