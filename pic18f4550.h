#ifndef PIC18F4550_H_
#define PIC18F4550_H_

#include<stdint.h>

/* Unit micro seconds*/
#define DELAY_P1   	1
#define DELAY_P2   	1
#define DELAY_P2A  	1
#define DELAY_P2B  	1
#define DELAY_P3   	1
#define DELAY_P4   	1
#define DELAY_P5   	1
#define DELAY_P5A  	1
#define DELAY_P6   	1
#define DELAY_P9  	4000 // 4ms
#define DELAY_P10  	50000 // 50 ms
#define DELAY_P11  	500000 // 500ms
#define DELAY_P11A  40000 // 500ms
#define DELAY_P12  	400 // 400 us
#define DELAY_P13  	1
#define DELAY_P14  	1
#define DELAY_P15  	4
#define DELAY_P16  	1
#define DELAY_P17  	1
#define DELAY_P18	1
#define DELAY_P19	4000
#define DELAY_P20	1

/* commands for programming */
#define COMM_CORE_INSTRUCTION 				0x00 // Actually it's 4 bits 0b0000
#define COMM_SHIFT_OUT_TABLAT 				0x02
#define COMM_TABLE_READ	     				0x08
#define COMM_TABLE_READ_POST_INC 			0x09
#define COMM_TABLE_READ_POST_DEC			0x0A
#define COMM_TABLE_READ_PRE_INC 			0x0B
#define COMM_TABLE_WRITE					0x0C
#define COMM_TABLE_WRITE_POST_INC_2			0x0D
#define COMM_TABLE_WRITE_STARTP_POST_INC_2	0x0E
#define COMM_TABLE_WRITE_STARTP				0x0F

#define ENTER_PROGRAM_KEY	0x4D434850

void setup_io();
void enter_program_mode(void);
void exit_program_mode(void);
int read_device_id(void);
void bulk_erase(void);
void dump_configuration_registers(void);
void read1(char *outfile, uint32_t start, uint32_t count);
void write1(char *infile);
uint8_t blank_check(void);

#endif
