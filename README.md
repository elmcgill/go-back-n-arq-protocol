# go-back-n-arq-protocol

In this lab experiment, you are required to design and develop a sender function (primary.c) to implement the GoBack-N ARQ protocol, with a Primary function (which will act as a client or sender) while the Secondary function
(which will act as a server or receiver) is given to you. This Go-Back-N ARQ protocol is a revised version from the
one we learned in class. One difference is that it uses both ACK and NAK packets; this is to simplify the
implementation and avoid having to implement the timeout mechanism.

## Primary Function:
* Send the alphabet, ABCDEFGHIJKLMNOPQRSTUVWXYZ, to the Secondary in a total of 13 packets (2
characters per packet, NOTE: the total number of packets sent may be higher since the packets may be
corrupted by IntroduceError() and thus need to be redelivered).
* Use function “buildPacket” that found in “utilities.c” to build a packet of the following
format:
Packet Type | Packet Number | Data | CRC
----------- | ------------- | ---- | ---
1 byte | 1 byte | 2 bytes | 2 bytes

* Packet Type
  * 1: Data Packet(sent from Primary to Secondary)
  * 2: Acknowledgement Packet (ACK) (sent from Secondary to Primary)
  * 3: Negative Acknowledgment Packet (NAK) (sent from Secondary to Primary)
* Packet Number
  * Starts from 0 and increments sequentially to 12
* Data
  * Two alphabet characters (sent from Primary to Secondary)
  * No data is sent from Secondary to Primary
* CRC
  * CRC generated for this entire packet, including Packet Type, Packet Number, and Data fields
* Display the packet. (printPacket(.))
* Apply the “IntroduceError(., .)” routine to the entire packet. Pass the BER value to the program
as an argument in the command line.
* Implement the Go-Back-N ARQ protocol with a send window of size N = 3

## Secondary Function:
* Accept packers from the Primary
* Run the CRC check
  * If the packet is received error free and in sequence, it displays the packet content and sequence
number, then sends back an ACK.
  * If the packet is received error free but out of sequence, it does not display the packet content but
displays the sequence number, then sends back an ACK.
  * If the packet is received in error, it does not display the packet content but displays the sequence
number if possible, then sends back a NAK.
