all: sender receiver

sender: primary.c
	gcc -c primary.c
	gcc -c introduceerror.c
	gcc -o sender sender.o primary.o introduceerror.o ccitt16.o utilities.o

receiver: *
	gcc -c secondary.c
	gcc -c utilities.c
	gcc -o receiver receiver.o secondary.o utilities.o ccitt16.o
