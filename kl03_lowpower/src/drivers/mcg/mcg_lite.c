/*
 * File:    mcg.c
 *
 * Notes:
 * Assumes the MCG-lite mode is in the default LIRC mode out of reset
 */
#include "common.h"
#include "mcg_lite.h"


unsigned long MCG_HIRC48(void);

unsigned long MCG_HIRC48(void)
{
    unsigned long i;
    MCG_MC |= MCG_MC_HIRCEN_MASK;
    MCG_C1 = MCG_C1_CLKS(0);
    
    i = 0;
    while(((MCG_S&MCG_S_CLKST_MASK)!=0) && (i<20000))
    {
        i++;
    }
    if(i<20000)
    {
        return 48000000;
    }
    else
    {
        return 0;
    }
}




