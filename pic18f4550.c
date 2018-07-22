#include "./pic18f4550.h"
#include "./raspi.h"
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <poll.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/ioctl.h>


#define ENTER_PROGRAM_KEY	0x4D434850

void goto_mem_location(uint32_t data);
void send_cmd(uint8_t cmd);


/**
 * Implementation Header File functions.
 * */

void setup_io() {
   pinMode (DEFAULT_PIC_CLK, OUTPUT) ;
   pinMode (DEFAULT_PIC_PGM, OUTPUT) ;
   pinMode (DEFAULT_PIC_MCLR, OUTPUT) ;
   pinMode (DEFAULT_PIC_DOUT, OUTPUT) ;
   pinMode (DEFAULT_PIC_DIN, INPUT) ;
}

void enter_program_mode() {
    delayMicroseconds(1000);
    digitalWrite(DEFAULT_PIC_PGM, HIGH);
    delayMicroseconds(DELAY_P15);
    digitalWrite(DEFAULT_PIC_MCLR, HIGH);;
    delayMicroseconds(DELAY_P12);
    digitalWrite(DEFAULT_PIC_DOUT, HIGH);
}

void exit_program_mode() {
    digitalWrite(DEFAULT_PIC_CLK, LOW);			/* stop clock on PGC */
	digitalWrite(DEFAULT_PIC_DOUT, LOW);			/* clear data pin PGD */
	delayMicroseconds(DELAY_P16);	/* wait P16 */
	digitalWrite(DEFAULT_PIC_MCLR, LOW);			/* remove VDD from MCLR pin */
	delayMicroseconds(DELAY_P18);	/* wait (at least) P17 */
    digitalWrite(DEFAULT_PIC_PGM, LOW);
}


/* 8 bit read */
uint16_t read_data(void) {
    uint8_t i;
	uint16_t data = 0x0000;

	for (i = 0; i < 8; i++) {
		digitalWrite(DEFAULT_PIC_CLK, HIGH);
		delayMicroseconds(DELAY_P2B);
		digitalWrite(DEFAULT_PIC_CLK, LOW);
		delayMicroseconds(DELAY_P2A);
	}

	delayMicroseconds(DELAY_P6);	/* wait for the data... */

	for (i = 0; i < 8; i++) {
		digitalWrite(DEFAULT_PIC_CLK, HIGH);
		delayMicroseconds(DELAY_P14);	/* Wait for data to be valid */
		data |= (digitalRead(DEFAULT_PIC_DIN) & 0x00000001 ) << i;
		delayMicroseconds(DELAY_P2B);
		digitalWrite(DEFAULT_PIC_CLK, LOW);
		delayMicroseconds(DELAY_P2A);
	}

	delayMicroseconds(DELAY_P5A);

	return data;
}
/* Write data*/
void write_data(uint16_t data){
	int i;

	for (i = 0; i < 16; i++) {

		digitalWrite(DEFAULT_PIC_CLK, HIGH);
		if ( (data >> i) & 0x0001 )
            digitalWrite(DEFAULT_PIC_DOUT, HIGH);
		else
			digitalWrite(DEFAULT_PIC_DOUT, LOW);

		delayMicroseconds(DELAY_P2B);	/* Setup time */
		digitalWrite(DEFAULT_PIC_CLK, LOW);
		delayMicroseconds(DELAY_P2A);	/* Hold time */
	}
	digitalWrite(DEFAULT_PIC_DOUT, LOW);
	delayMicroseconds(DELAY_P5A);
}

int read_device_id() {
    uint16_t id = -1;
    uint16_t id2 = -1;
	int found = 0;

	goto_mem_location(0x3FFFFE);

	send_cmd(COMM_TABLE_READ_POST_INC);
	id = read_data();
	

	send_cmd(COMM_TABLE_READ_POST_INC);
    id2 = read_data();
	id = ( id2 << 8) | id;

    printf("Device_id read as: 0x%x\n", id);
	return 0x1207 == id;
}

uint8_t check_if_device_is_blank(void) {

}

void bulk_erase() {

}

void read1(char *outfile, uint32_t start, uint32_t count) {

}

void write1(char *infile) {

}

void display_config_registers() {

}


void send_cmd(uint8_t cmd)
{
	int i;
	for (i = 0; i < 4; i++) {
		digitalWrite(DEFAULT_PIC_CLK, HIGH);
		if ( (cmd >> i) & 0x01 )
            digitalWrite(DEFAULT_PIC_DOUT, HIGH);
		else
			digitalWrite(DEFAULT_PIC_DOUT, LOW);
		delayMicroseconds(DELAY_P2B);	/* Setup time */
		digitalWrite(DEFAULT_PIC_CLK, LOW);
		delayMicroseconds(DELAY_P2A);	/* Hold time */
	}
	digitalWrite(DEFAULT_PIC_DOUT, LOW);
	delayMicroseconds(DELAY_P5);
}

void goto_mem_location(uint32_t data)
{

	data = data & 0x00FFFFFF;	/* set the MSB byte to zero (it should already be zero)	*/

	send_cmd(COMM_CORE_INSTRUCTION);
	write_data( 0x0E00 | ( (data >> 16) & 0x000000FF) );/* MOVLW Addr[21:16] */
	send_cmd(COMM_CORE_INSTRUCTION);
	write_data(0x6EF8);					/* MOVWF TBLPTRU */
	send_cmd(COMM_CORE_INSTRUCTION);
	write_data( 0x0E00 | ( (data >> 8) & 0x000000FF) );	/* MOVLW Addr[15:8] */
	send_cmd(COMM_CORE_INSTRUCTION);
	write_data(0x6EF7);					/* MOVWF TBLPTRH */
	send_cmd(COMM_CORE_INSTRUCTION);
	write_data( 0x0E00 | (data & 0x000000FF) );		/* MOVLW Addr[7:0] */
	send_cmd(COMM_CORE_INSTRUCTION);
	write_data(0x6EF6);					/* MOVWF TBLPTRL */
}

void read_config_registers() {
    uint16_t id = -1;
    uint16_t id2 = -1;
	int found = 0;

	goto_mem_location(0x300000);

	for(int i = 1; i <= 7; i++) {
		send_cmd(COMM_TABLE_READ_POST_INC);
		id = read_data();
		send_cmd(COMM_TABLE_READ_POST_INC);
		id2 = read_data();
		id = ( id2 << 8) | id;
		printf("Config_%d read as: 0x%04x\n", i, id);
	}
}