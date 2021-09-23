/*
*	swap_server.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define	MAXLINE	128	// maximum characters to receive and send at once
#define MAXFRAME 256

extern int swap_accept(unsigned short port);
extern int swap_disconnect(int sd);
extern int sdp_send(int sd, char *buf, int length);
extern int sdp_receive(int sd, char *buf);
extern int sdp_receive_with_timer(int sd, char *buf, unsigned int expiration);
extern short checksum(unsigned char buf[], int length);

int session_id = 0;
int R = 0;	// frame number to receive

int swap_wait(unsigned short port)
{
	/*
	*	if the session is already open, then return error
	*/

	if (session_id != 0)
		return -1;

	/*
	*	accept a connection
	*/

	session_id = swap_accept(port);	// in sdp.open

	/*
	*	return a ssession id
	*/

	return session_id;
}

int swap_read(int sd, char *buf)
{
	int	n;
	char frame[MAXFRAME];
    

	if (session_id == 0 || sd != session_id)
		return -1;

	/*
	*	receive a frame without a timer
	*/
    n = sdp_receive(sd, frame);

	/*
	*	several cases
	*/

	
	/*
	*	copy the data field in the frame into buf, and return the length
	*/
    bzero(buf, sizeof(buf));
    strcpy(buf, frame);
    return sizeof(buf);
	
}

void swap_close(int sd)
{
	if (session_id == 0 || sd != session_id)
		return;

	else
		session_id = 0;

	swap_disconnect(sd);	// in sdp.o
}
