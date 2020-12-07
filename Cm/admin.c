#ifndef TEST

/* admin.c - admin for the Cm Embedded Virtual Machine which:
//         - isolates the <stdio.h> with all put* in the VM
//         - defines _CRT_SECURE_NO_WARNINGS to avoid all MS secure crap on **_s
//
// Copyright (C) 1999-2020 by Michel de Champlain
//
*/

#include "hal.h"
#include "hal_out.h"

#ifdef SERIAL_LOADER
#include "serial_loader.h"
#elif FILE_LOADER
#include "file_loader.h"
#endif

#include "vm.h"

int main(int argc, char* argv[]) {

    Hal_Init();
    enter_loader_main_loop(argc, argv);

    return 0;

}

#endif