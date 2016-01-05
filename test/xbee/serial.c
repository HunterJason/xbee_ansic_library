/*
 * Copyright (c) 2010-2012 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */

//FIXME: this currently assumes DOS.  Needs work to make platform-independent.

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "xbee/platform.h"
#include "xbee/cbuf.h"
#include "xbee/serial.h"

//#include <conio.h>

#ifndef USE_IMAX257
xbee_serial_t ser = {115200, 0, "/dev/ttyUSB1"};
#else
xbee_serial_t ser = {115200, 0, "/dev/ttymxc2"};
#endif


/*
	main

	Open serial port, then send keystrokes to it and print received chars.
	This is a very basic test of serial port functionality.

	At the simplest, you can loop back Tx and Rx on the wire, and whatever
	you type in should get echoed.
*/


int main( int argc, char *argv[])
{
	int rc;
	char cmdstr[255];
	char readstr[255];
	int num;
#if 0		// Variable COM port supported...
	int i, rc;
	char buffer[80];

	for (i = 1; i < argc; ++i)
	{
		if (strncmp( argv[i], "COM", 3) == 0)
		{
			xbee->comport = (int) strtoul( argv[i] + 3, NULL, 10);
		}
		if ( (baud = (uint32_t) strtoul( argv[i], NULL, 0)) > 0)
		{
			xbee->baudrate = baud;
		}
	}

	while (xbee->comport == 0)
	{
		printf( "Connect to which COMPORT? ");
		fgets( buffer, sizeof buffer, stdin);
		xbee->comport = (int) strtoul( buffer, NULL, 10);
	}
#endif

	rc = xbee_ser_open(&ser, 9600);	// Baud rate

	if (rc)
	{
		printf("Failed to open serial port rc=%d\n", rc);
		exit(1);
	}


	printf("Characters will be echoed if loopback.  Start typing.\n");
	printf("Press ESC to exit.\n");
	printf("Press ? to print stats.\n");

	while (1)
	{
		while (xbee_readline( cmdstr, sizeof cmdstr) == -EAGAIN)
		{
			num = xbee_ser_read(&ser, readstr, 255);
			if(num > 0)
			{
				printf("read num :%d ,data is:%s\r\n",num, readstr);
				memset(readstr,0,sizeof(readstr));
			}
			
		}
		if (! strcmpi( cmdstr, "quit"))
		{
			xbee_ser_close(&ser);
			return 0;
		}

		printf("write string :%s, len is:%d\r\n",cmdstr,(int)strlen(cmdstr));
		num = xbee_ser_write(&ser, cmdstr, strlen( cmdstr) );
		printf("write num :%d\r\n",num);
	}
	xbee_ser_close(&ser);
	return 0;
}

