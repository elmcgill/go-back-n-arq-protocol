#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include "ccitt16.h"
#include "utilities.h"
#include "introduceerror.h"

void primary(int sockfd, double ber) {
	int readSize;
  char msg[DATA_LENGTH], srvReply[150];
	unsigned char packet[PACKET_SIZE];
	unsigned int packetNum = 0;
	struct packet_buffer buffer[50];
	int sendWindow[WINDOW_SIZE] = {0,1,2};
	//Initialize srvReply to be "empty" for the beginning
	srvReply[0] = 0;

    //keep communicating with server
    while(1)
    {
        printf("Enter message : ");
		// fgets will load only DATA_LENGTH characters into msg at a time
		// however, if the user enters more than DATA_LENGTH characters,
		// the remaining characters will be loaded in DATA_LENGTH at a time
		// with each iteration of the loop
		//
		// ie if the user enters "HELLO<enter>", fgets will read in "HE", then
		// packetize it and send it. The program will come back to fgets and
		// read in "LL" and repeat until it sends "O"
		fgets(msg, DATA_LENGTH+1, stdin);
		// secondary is waiting for a *packet*, so we should packetize what
		// we're sending to it
		buildPacket(packet, DATA_PACKET, msg, packetNum++);

		struct packet_buffer pack = {packet};
		//Store the error free packet
		buffer[packetNum-1] = pack;

		printPacket(packet);

		//Introduce error before sending packets
		IntroduceError(packet, ber);

		// 0 - Options and are not necessary here
		// If return value < 0, an error occured

		//Sending the first three frames
		if(srvReply[0] == 0){
			if( send(sockfd , packet, PACKET_SIZE, 0) < 0)
						perror("Send failed");
		}
		//Receive an ack
		else if (srvReply[0] == '2'){
			shiftWindow(sendWindow, WINDOW_SIZE, 1);
			//shiftBuf(buffer, 50, 1);
			if( send(sockfd , packet, PACKET_SIZE, 0) < 0)
						perror("Send failed");
		}
		//Receive a nak
		else if (srvReply[0] == '3'){
			//Retransmit the entire send window
			int i;
			for(i = 0; i<3; i++){
				if( send(sockfd , buffer + sendWindow[i], PACKET_SIZE, 0) < 0){
					perror("Send failed");
				}
			}
		}
        // Receive a reply from the server
		// NOTE: If you have more data than 149 bytes, it will
		// 			be received in the next call to "recv"
		// readSize - the length of the received message
		// 				or an error code in case of failure
		// msg - a buffer where the received message will be stored
		// 149 - the size of the receiving buffer (any more data will be
		// 		delievered in subsequent "recv" operations
		// 0 - Options and are not necessary here
			if( packetNum >= 2){
				if( (readSize = recv(sockfd , srvReply , 149 , 0)) < 0)
            perror("recv failed");
			}

		// In general, secondary responds with only ACK or NAK *packets*
		// so we should print them with printPacket(x)
        printf("Server's reply:\n");
		printPacket(srvReply);
    }

}
