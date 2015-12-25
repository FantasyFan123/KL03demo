/********************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2010 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
**************************************************************************//*!
 *
 * @file flash_FTFA.c
 *
 * @author
 *
 * @version
 *
 * @date
 *
 * @brief The file includes flash routines
 *
 *****************************************************************************/

/*********************************** Includes ***********************************/

//#include "derivative.h" /* include peripheral declarations */
#include "flash_FTFL.h" /* include flash driver header file */
/*********************************** Macros ************************************/

/*********************************** Defines ***********************************/

/********************************** Constant ***********************************/

/*********************************** Variables *********************************/

/*********************************** Prototype *********************************/

/*********************************** Function **********************************/


/*******************************************************************************
 * Function:        Flash_Init
 *
 * Description:     Set the flash clock
 *
 * Returns:         never return
 *
 * Notes:
 *
 *******************************************************************************/
void Flash_Init(unsigned char FlashClockDiv)
{
    /* Body */
//    UNUSED(FlashClockDiv)
    /* checking access error */
    if (FTFA_FSTAT & FTFA_FSTAT_ACCERR_MASK)
    {
        /* clear error flag */
        FTFA_FSTAT |= FTFA_FSTAT_ACCERR_MASK;
    }
    /* checking protection error */
    else if (FTFA_FSTAT & FTFA_FSTAT_FPVIOL_MASK)
    {
        /* clear error flag */
        FTFA_FSTAT |= FTFA_FSTAT_FPVIOL_MASK;
    }
    else if (FTFA_FSTAT & FTFA_FSTAT_RDCOLERR_MASK)
    {
        /* clear error flag */
        FTFA_FSTAT |= FTFA_FSTAT_RDCOLERR_MASK;
    } /* EndIf */
    /* Disable Data Cache in Flash memory Controller Module  */
#if (defined (_MK_xxx_H_))
    FMC_PFB0CR &= ~FMC_PFB0CR_B0DCE_MASK;
    FMC_PFB1CR &= ~FMC_PFB1CR_B1DCE_MASK;
#endif
} /* EndBody */


/*******************************************************************************
 * Function:        Flash_SectorErase
 *
 * Description:     erase a sector of the flash
 *
 * Returns:         Error Code
 *
 * Notes:
 *
 *******************************************************************************/
unsigned char Flash_SectorErase(uint32_t FlashPtr)
{
    /* Body */
    unsigned char Return = Flash_OK;

    /* Allocate space on stack to run flash command out of SRAM */
    /* wait till CCIF is set*/
    while (!(FTFA_FSTAT & FTFA_FSTAT_CCIF_MASK)){};
    /* Write command to FCCOB registers */
    FTFA_FCCOB0 = FlashCmd_SectorErase;
    FTFA_FCCOB1 = (uint8)(FlashPtr >> 16);
    FTFA_FCCOB2 = (uint8)((FlashPtr >> 8) & 0xFF);
    FTFA_FCCOB3 = (uint8)(FlashPtr & 0xFF);

    /* Launch command */
    FTFA_FSTAT |= FTFA_FSTAT_CCIF_MASK;    
  
    /* wait for command completion */
    while (!(FTFA_FSTAT & FTFA_FSTAT_CCIF_MASK)) {};
    /* checking access error */
    if (FTFA_FSTAT & FTFA_FSTAT_ACCERR_MASK)
    {
        /* clear error flag */
        FTFA_FSTAT |= FTFA_FSTAT_ACCERR_MASK;

        /* update return value*/
        Return |= Flash_FACCERR;
    }
    /* checking protection error */
    else if (FTFA_FSTAT & FTFA_FSTAT_FPVIOL_MASK)
    {
        /* clear error flag */
        FTFA_FSTAT |= FTFA_FSTAT_FPVIOL_MASK;

        /* update return value*/
        Return |= Flash_FPVIOL;
    }
    else if (FTFA_FSTAT & FTFA_FSTAT_RDCOLERR_MASK)
    {
        /* clear error flag */
        FTFA_FSTAT |= FTFA_FSTAT_RDCOLERR_MASK;

       /* update return value*/
       Return |= Flash_RDCOLERR;
    }
    /* checking MGSTAT0 non-correctable error */
    else if (FTFA_FSTAT & FTFA_FSTAT_MGSTAT0_MASK)
    {
        Return |= Flash_MGSTAT0;
    } /* EndIf */
    /* function return */
    return  Return;
} /* EndBody */


/*******************************************************************************
 * Function:        Flash_ByteProgram
 *
 * Description:     byte program the flash
 *
 * Returns:         Error Code
 *
 * Notes:
 *
 *******************************************************************************/
unsigned char Flash_ByteProgram(uint32_t FlashStartAdd,uint32_t *DataSrcPtr,uint32_t NumberOfBytes)
{
    /* Body */
    unsigned char cReturn = Flash_OK;
    /* Allocate space on stack to run flash command out of SRAM */
    uint32_t size_buffer;
    if (NumberOfBytes == 0)
    {
        return Flash_CONTENTERR;
    }
    else
    {
        size_buffer = (NumberOfBytes - 1)/4 + 1;
    } /* EndIf */
	/* wait for command completion */
	while (!(FTFA_FSTAT & FTFA_FSTAT_CCIF_MASK)) {};
    while( size_buffer && (cReturn == Flash_OK) )
    {
		 /* Write command to FCCOB registers */
	    FTFA_FCCOB0 = FlashCmd_ProgramLongWord;
	    FTFA_FCCOB1 = (uint8)(FlashStartAdd >> 16);
	    FTFA_FCCOB2 = (uint8)((FlashStartAdd >> 8) & 0xFF);
	    FTFA_FCCOB3 = (uint8)(FlashStartAdd & 0xFF);
	    FTFA_FCCOB4 = (uint8)(*((uint8*)DataSrcPtr+3));
		FTFA_FCCOB5 = (uint8)(*((uint8*)DataSrcPtr+2));
		FTFA_FCCOB6 = (uint8)(*((uint8*)DataSrcPtr+1));
		FTFA_FCCOB7 = (uint8)(*((uint8*)DataSrcPtr+0));
	    /* Launch command */
	    FTFA_FSTAT |= FTFA_FSTAT_CCIF_MASK;   
	    /* wait for command completion */
	    while (!(FTFA_FSTAT & FTFA_FSTAT_CCIF_MASK)) {};
	    /* checking access error */
	    if (FTFA_FSTAT & FTFA_FSTAT_ACCERR_MASK)
	    {
	        /* clear error flag */
	        FTFA_FSTAT |= FTFA_FSTAT_ACCERR_MASK;

	        /* update return value*/
	        cReturn |= Flash_FACCERR;
	    }
	    /* checking protection error */
	    else if (FTFA_FSTAT & FTFA_FSTAT_FPVIOL_MASK)
	    {
	        /* clear error flag */
	        FTFA_FSTAT |= FTFA_FSTAT_FPVIOL_MASK;

	        /* update return value*/
	        cReturn |= Flash_FPVIOL;
	    }
	    else if (FTFA_FSTAT & FTFA_FSTAT_RDCOLERR_MASK)
	    {
	        /* clear error flag */
	        FTFA_FSTAT |= FTFA_FSTAT_RDCOLERR_MASK;

	        /* update return value*/
	        cReturn |= Flash_RDCOLERR;
	    }
	    /* checking MGSTAT0 non-correctable error */
	    else if (FTFA_FSTAT & FTFA_FSTAT_MGSTAT0_MASK)
	    {
	        cReturn |= Flash_MGSTAT0;
	    } /* EndIf */
        /* decrement byte count */
         size_buffer --;
         (uint32 *)DataSrcPtr++;
         FlashStartAdd +=4;
    } /* EndWhile */
    /* function return */
    return  cReturn;
} /* EndBody */


/* EOF */