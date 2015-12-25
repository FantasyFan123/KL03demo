/*
 * File:	start.c
 * Purpose:	Kinetis start up routines. 
 *
 * Notes:		
 */

#include "start.h"
#include "common.h"
#include "sysinit.h"
#include "rcm.h"
#include "startup.h"
/********************************************************************/
/*!
 * \brief   Kinetis Start
 * \return  None
 *
 * This function calls all of the needed starup routines and then 
 * branches to the main process.
 */
 
 void SystemInit (void)
{
	 start();
}
void start(void)
{ 
  /* Disable the watchdog timer */
  SIM_COPC = 0x00;
  /* Copy any vector or data sections that need to be in RAM */
#ifndef CMSIS    // If conforming to CMSIS, we do not need to perform this code
#ifndef BOOTLOADER
	/* Copy any vector or data sections that need to be in RAM */
	common_startup();
#endif
#endif
  /* Perform processor initialization */
  sysinit();
#ifndef BOOTLOADER      
//  printf("                \r\n");
  /* Determine the last cause(s) of reset */
  outSRS();	
  /* Determine specific Kinetis device and revision */
  cpu_identify();
#endif	

/* in KEIL, should not call main, but in starup_MKL05Z4.s Reset_Handler */
#ifndef KEIL    
  /* Jump to main process */
  main();
  /* No actions to perform after this so wait forever */
  while(1);
#endif
}
/********************************************************************/
/*!
 * \brief   Kinetis Identify
 * \return  None
 *
 * This is primarly a reporting function that displays information
 * about the specific CPU to the default terminal including:
 * - Kinetis family
 * - package
 * - die revision
 * - P-flash size
 * - Ram size
 */
void cpu_identify (void)
{
  uint16 temp;
    /* Determine the Kinetis family */
    switch((SIM_SDID & SIM_SDID_FAMID(0x7))>>SIM_SDID_FAMID_SHIFT) 
    {  
    	case 0x0:
    		printf("\nKL0");
    		break;
    	case 0x1:
    		printf("\nKL1");
    		break;
    	case 0x2:
    		printf("\nKL2");
    		break;
    	case 0x3:
    		printf("\nKL3");
    		break;
    	case 0x4:
    		printf("\nKL4");
    		break;
	default:
		printf("\nUnrecognized Kinetis family device.\n");  
		break;  	
    }
    
    /* Determine Sub-Family ID */
    switch((SIM_SDID & SIM_SDID_SUBFAMID(0x7))>>SIM_SDID_SUBFAMID_SHIFT) 
    {  
      case 0x2:
    		printf("2");
    		break;
      case 0x3:
    		printf("3");
    		break;
    	case 0x4:
    		printf("4");
    		break;
    	case 0x5:
    		printf("5");
    		break;
      case 0x6:
    		printf("6");
    		break;
	    default:
		    printf("\nUnrecognized Kinetis sub-family device.\n");  
		    break;  	
    }
    
    printf("  ");
    /* Determine the package size */
    switch((SIM_SDID & SIM_SDID_PINID(0xF))>>SIM_SDID_PINID_SHIFT) 
    {  
        case 0x0:
    		printf("16pin       ");
    		break;
        case 0x1:
    		printf("24pin       ");
    		break; 
        case 0x2:
    		printf("32pin       ");
    		break;      
        case 0x4:
    		printf("48pin       ");
    		break;
    	case 0x5:
    		printf("64pin       ");
    		break;
    	case 0x6:
    		printf("80pin       ");
    		break;
    	case 0x7:
    		printf("81pin       ");
    		break;
    	case 0x8:
    		printf("100pin      ");
    		break;
    	case 0x9:
    		printf("121pin      ");
    		break;
    	case 0xA:
    		printf("144pin      ");
    		break;
	default:
		printf("\nUnrecognized Kinetis package code.      ");  
		break;  	
    }
    
    /* Determine Attribute ID */
   /*
    switch((SIM_SDID & SIM_SDID_ATTRID(0x7))>>SIM_SDID_ATTRID_SHIFT) 
    {  
    	case 0x1:
    		printf(" Low Power Line with Cortex M0+\n\n");
    		break;
	default:
		printf("\nUnrecognized Kinetis family attribute.\n");  
		break;  	
    }
    */
    /* Determine the System SRAM Size */
    switch((SIM_SDID & SIM_SDID_SRAMSIZE(0x7))>>SIM_SDID_SRAMSIZE_SHIFT) 
    {  
    	case 0x0:
          printf("SRAM Size: 0.5 KB\n");
    		break;
        case 0x1:
          printf("SRAM Size:  1 KB\n");
          break;
        case 0x2:
          printf("SRAM Size:  2 KB\n");
          break;
        case 0x3:
          printf("SRAM Size:  4 KB\n");
          break;
        case 0x4:
          printf("SRAM Size:  8 KB\n");
          break;
        case 0x5:
          printf("SRAM Size:  16 KB\n");
          break;
        case 0x6:
          printf("SRAM Size:  32 KB\n");
          break;
        case 0x7:
          printf("SRAM Size:  64 KB\n");
          break;
	default:
		printf("\nUnrecognized SRAM Size.\n");  
		break;  	
    }                

    /* Determine the revision ID */
    temp = ((SIM_SDID_REVID(0xF))>>SIM_SDID_REVID_SHIFT);
    printf("Silicon rev %d\n", temp);
    
    /* Determine the flash revision */
//    flash_identify();    
    
    /* Determine the P-flash size */
    switch((SIM_FCFG1 & SIM_FCFG1_PFSIZE(0xF))>>SIM_FCFG1_PFSIZE_SHIFT)
    {
  	case 0x0:
    		printf("Flash size:  8 KB program flash, 0.25 KB protection region\n");
    		break;
    	case 0x1:
    		printf("Flash size:  16 KB program flash, 0.5 KB protection region\n");
    		break;
        case 0x3:
    		printf("Flash size:  32 KB program flash, 1 KB protection region\n");
    		break;
    	case 0x5:
    		printf("Flash size:  64 KB program flash, 2 KB protection region\n");
    		break;
        case 0x7:
    		printf("Flash size:  128 KB program flash, 4 KB protection region\n");
    		break;
        case 0x9:
    		printf("Flash size:  256 KB program flash, 8 KB protection region\n");
    		break;
        case 0xF:
    		printf("Flash size:  32 KB program flash, 1 KB protection region\n");
    		break;
	default:
		printf("ERR!! Undefined flash size\n");  
		break;  	  		
    }
}
/********************************************************************/
/*!
 * \brief   flash Identify
 * \return  None
 *
 * This is primarly a reporting function that displays information
 * about the specific flash parameters and flash version ID for 
 * the current device. These parameters are obtained using a special
 * flash command call "read resource." The first four bytes returned
 * are the flash parameter revision, and the second four bytes are
 * the flash version ID.
 */
void flash_identify (void)
{
    /* Get the flash parameter version */

    /* Write the flash FCCOB registers with the values for a read resource command */
    FTFA_FCCOB0 = 0x03;
    FTFA_FCCOB1 = 0x00;
    FTFA_FCCOB2 = 0x00;
    FTFA_FCCOB3 = 0x00;
    FTFA_FCCOB8 = 0x01;

    /* All required FCCOBx registers are written, so launch the command */
    FTFA_FSTAT = FTFA_FSTAT_CCIF_MASK;

    /* Wait for the command to complete */
    while(!(FTFA_FSTAT & FTFA_FSTAT_CCIF_MASK));
    
//    printf("Flash parameter version %d.%d.%d.%d\n",FTFA_FCCOB4,FTFA_FCCOB5,FTFA_FCCOB6,FTFA_FCCOB7);

    /* Get the flash version ID */   

    /* Write the flash FCCOB registers with the values for a read resource command */
    FTFA_FCCOB0 = 0x03;
    FTFA_FCCOB1 = 0x00;
    FTFA_FCCOB2 = 0x00;
    FTFA_FCCOB3 = 0x04;
    FTFA_FCCOB8 = 0x01;

    /* All required FCCOBx registers are written, so launch the command */
    FTFA_FSTAT = FTFA_FSTAT_CCIF_MASK;

    /* Wait for the command to complete */
    while(!(FTFA_FSTAT & FTFA_FSTAT_CCIF_MASK));

//    printf("Flash version ID %d.%d.%d.%d\n",FTFA_FCCOB4,FTFA_FCCOB5,FTFA_FCCOB6,FTFA_FCCOB7);  
}
/********************************************************************/

