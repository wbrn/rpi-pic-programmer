#ifndef RASPI_H_
#define RASPI_H_

#include<stdint.h>

#define DEFAULT_PIC_MCLR   1   /* MCLR - Output */
#define DEFAULT_PIC_PGM    3   /* PGM - Output */
#define DEFAULT_PIC_CLK    2   /* PGC - Output */
#define DEFAULT_PIC_DOUT   4   /* PGDout - Output */
#define DEFAULT_PIC_DIN    5   /* PGIn - Input */

void dummy_code();
#endif
