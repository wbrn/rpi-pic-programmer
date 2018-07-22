ALL:
	rm -f ./a.out
	gcc main.c pic18f4550.c raspi.c -lwiringPi
	sudo ./a.out