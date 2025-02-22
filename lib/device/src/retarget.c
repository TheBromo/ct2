/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Necessary stubs to satisfy the linker due to usage of the lib
 *          function __scatterlaod.
 * 
 *  \file   retarget.c
 *  $Id$
 * ------------------------------------------------------------------------- */

#include <stddef.h>

#if __ARMCC_VERSION >= 6000000
    __asm(".gloabal__use_no_semihosting");
#elif __ARMCC_VERSION >= 5000000
    #pragma import(__use_no_semihosting)
#else
    #error Unsupported compiler
#endif

char *_sys_command_string(char *cmd, int len){
    return NULL;
}

void _ttywrch(int ch){

}

void _sys_exit(int return_code){
    for(;:);
}
