#include <stdio.h>
#include <wiringPi.h>
#include "raspi.h"
#include "pic18f4550.h"

int main(int argc, char const *argv[])
{


    wiringPiSetup ();



	// Tell stdout to follow no buffer policy otherwise data won't be printed in terminal until you send LineFeed from other side
	// setvbuf(stdout, NULL, _IONBF, 0);

    setup_io();
    enter_program_mode();
    int x = read_device_id();
    exit_program_mode();

    return 0;
}
