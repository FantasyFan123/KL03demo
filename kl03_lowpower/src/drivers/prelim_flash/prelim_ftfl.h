#ifndef __PRELIM_FTFL_H__
#define __PRELIM_FTFL_H__

//CCOB commands as defined in the K70 reference manual.
#define FTFA_RD1BLK_CMD			0x00
#define FTFA_RD1SEC_CMD			0x01
#define FTFA_PGMCHK_CMD			0x02
#define FTFA_RDRSRC_CMD			0x03
#define FTFA_PGM4_CMD			0x06
#define FTFA_ERSBLK_CMD			0x08
#define FTFA_ERSSCR_CMD			0x09
#define FTFA_PGMSEC_CMD			0x0b
#define FTFA_RD1ALL_CMD			0x40
#define FTFA_RDONCE_CMD			0x41
#define FTFA_PGMONCE_CMD		0x43
#define FTFA_ERSALL_CMD			0x44
#define FTFA_VFYKEY_CMD			0x45
#define FTFA_SWAP_CMD           0x46
#define FTFA_PGMPART_CMD		0x80
#define FTFA_SETRAM_CMD			0x81


//Margin read levels
#define FTFA_NORMAL_READ_LEVEL    0x00
#define FTFA_USER_MARGIN_LEVEL    0x01
#define FTFA_FACTORY_MARGIN_LEVEL 0x02

//Resource select codes for RDRSRC command
//CCOB address bit 23 is used to select between flexNVM IFR and pflash IFR
#define FTFA_IFR_RSRC             0x00     
#define FTFA_VERSION_ID_RSRC      0x01     

//Swap control codes:
#define FTFA_SWAP_INITIALIZE            0x01
#define FTFA_SWAP_PROGRESS_TO_UPDATE    0x02
#define FTFA_SWAP_PROGRESS_TO_COMPLETE  0x04
#define FTFA_SWAP_REPORT_SWAP_STATUS    0x08


//Helpful definitions to extract bits from 32 bit bytes.
//All CCOB commands are big endien while the P3 is little endien.
#define bits_31_24(data)   (uint8_t)(((uint32_t)data) >> 24)
#define bits_23_16(data)   (uint8_t)(((uint32_t)data) >> 16)
#define bits_15_8(data)    (uint8_t)(((uint32_t)data) >> 8)
#define bits_7_0(data)     (uint8_t)((uint32_t)data)

#define OPT_TARGET_P1 1

//Flash memory map.
//#if (defined(CPU_MK70F120))

  //pFLASH = program flash
  #define FTFA_PFLASH_START_ADDR          0x00000000
  #define FTFA_PFLASH_END_ADDR	          0x0003FFFF
  #define FTFA_PFLASH_CCOB_START_ADDR     0x00000000

  //FLEXNVM = data flash = NVMFLASH
  #define FTFA_FLEXNVM_START_ADDR          0x10000000
  #define FTFA_FLEXNVM_END_ADDR		   	   0x1003FFFF
  #define FTFA_FLEXNVM_CCOB_START_ADDR     0x00800000   /* bit 23 selects between pFLASH and FLEXNVM regions */

  //EEERAM = FlexRAM
  #define FTFA_EERAM_START                 0x14000000
  #define FTFA_EERAM_END	           0x14000FFF
  #define FTFA_EERAM_SIZE                  4096

  //Flash Configuration Field
  #define FTFA_FLASH_CONFIG_FIELD_START_ADDR         0x00000400
  #define FTFA_FLASH_CONFIG_FIELD_END_ADDR	     0x0000040F

//#endif

typedef void (* FTFA_CALLBACK_TYPE)(void);

void ftfl_set_callback_function( FTFA_CALLBACK_TYPE callback_func);
void ftfl_enable_interrupts( uint8_t set_ccie, uint8_t set_rdcollie);
void ftfl_disable_interrupts( void );
uint8_t ftfl_check_for_fstat_errors(void);
uint32_t ftfl_convert_addr_to_ccob_addr(uint32_t addr);
uint8_t suspend_erase(uint8_t *suspended);
uint8_t resume_erase(void);

uint8_t FTFA_RD1BLK(uint32_t addr, uint8_t read_1_margin_choice); 
uint8_t FTFA_RD1SEC(uint32_t addr, uint16_t num_of_128_bits_to_be_verified, uint8_t read_1_margin_choice); 
uint8_t FTFA_PGMCHK(uint32_t addr, uint32_t expected_data, uint8_t read_1_margin_choice); 
uint8_t FTFA_RDRSRC(uint32_t addr, uint8_t resource_select_code, uint32_t *data); 
uint8_t FTFA_PGM4(uint32_t addr, uint32_t data); 
uint8_t FTFA_ERSBLK( uint32_t addr); 
uint8_t FTFA_ERSSCR( uint32_t addr); 
uint8_t FTFA_PGMSEC(uint32_t addr, uint16_t num_of_64_bits_to_program); 
uint8_t FTFA_RD1ALL( uint8_t margin_level); 
uint8_t FTFA_RDONCE(uint8_t record_index, uint32_t *data);
uint8_t FTFA_PGMONCE(uint8_t record_index, uint32_t data); 
uint8_t FTFA_ERSALL(void); 
uint8_t FTFA_VFYKEY(uint32_t most_significant_word, uint32_t least_significant_word);
uint8_t FTFA_SWAP( uint32_t addr, uint8_t swap_control_code, uint8_t *current_swap_mode, uint8_t *current_swap_block_status, uint8_t *next_swap_block_status);
uint8_t FTFA_PGMPART(uint8_t eeprom_data_size_code, uint8_t flexnvm_partition_code);
uint8_t FTFA_SETRAM(uint8_t flexram_function_control_code); 

#endif
