

/* admin.c - admin for the Cm Embedded Virtual Machine which:
//         - isolates the <stdio.h> with all put* in the VM
//         - defines _CRT_SECURE_NO_WARNINGS to avoid all MS secure crap on **_s
//
// Copyright (C) 1999-2020 by Michel de Champlain
//
*/

#include "hal.h"
#include "hal_out.h"
#include "hal_loader.h"
#include "vm.h"


int main(int argc, char* argv[]) {

    Hal_Init();
    
    u8* mem = 0; // To avoid unitinializad local var used;

    while (true)
    {
        mem = hal_load_program(argc, argv);

        if (mem == 0)
        {
            VMOut_PutS("error: file too big");
            continue;
        }

        VM_Init(mem);
        VM_execute(mem);

        hal_unload_program();

#ifndef KEEP_ALIVE
        return 0;
#endif
    }

    return 0;
}


