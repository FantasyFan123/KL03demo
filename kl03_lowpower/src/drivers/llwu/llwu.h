/*!
 * \file    llwu_common.h
 * \brief   common LLWU defines
 *
 * This file defines the functions/interrupt handlers/macros used for LLWU.
 * And some common function prototypes and initializations.
 *
 * \version $Revision: 1.1 $
 * \author  Philip Drake[rxaa60]
 */

#ifndef __LLWU_COMMON_H__
#define __LLWU_COMMON_H__

/* 
 * Misc. Defines
 */
#define SIM_SCGC4_LLWU_MASK                      0x10000000u
#define SIM_SCGC4_LLWU_SHIFT                     28


#define LLWU_P0_PE       0x01          // PTA4, JP33 on L1PT EVB
#define LLWU_P1_PE       0x02          // PTA5, JP37 on L1PT EVB       
#define LLWU_P2_PE       0x04          // PTA6, JP38 on L1PT EVB      
#define LLWU_P3_PE       0x08          // PTA7, JP46 on L1PT EVB    //SW2 on L1PT TWR & EVB
#define LLWU_P4_PE       0x10          // PTB0, JP47 on L1PT EVB
#define LLWU_P5_PE       0x20          // PTB2, JP49 on L1PT EVB
#define LLWU_P6_PE       0x40          // PTB4, JP57 on L1PT EVB
#define LLWU_P7_PE       0x80          // PTA0, JP67 on L1PT EVB

#define LLWU_LPTMR0_ME      1          
#define LLWU_CMP0_ME        2          
#define LLWU_TSI0_ME        3          
#define LLWU_RTCS_ME        4         
#define LLWU_RTCA_ME        5         

#define LLWU_PIN_DIS        0
#define LLWU_PIN_RISING     1
#define LLWU_PIN_FALLING    2
#define LLWU_PIN_ANY        3
/* 
 * Function prototypes
 */

//

void llwu_configure(unsigned int pin_en, unsigned char rise_fall, unsigned char module_en);
void llwu_isr(void);
void llwu_configure_filter(unsigned int wu_pin_num, unsigned char filter_en, unsigned char rise_fall ); 

#endif /* __LLWU_COMMON_H__ */
