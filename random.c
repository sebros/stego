/*
 * random.c
 *
 *  Created on: May 7, 2014
 *      Author: Sebastian Rosenberg
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#include "types.h"
#include "debug.h"
#include "random.h"



unsigned int generateSeed(char *password)
{
	unsigned int crc = 0;
	char pwd[MAX_PASSWORD_LEN];

	if( NULL == password )
	{
		printf("Please insert password %d-%d characters: ", MIN_PASSWORD_LEN, MAX_PASSWORD_LEN);
		fgets(pwd, MAX_PASSWORD_LEN, stdin);

		DEBUG( ("Password scanned, length: %lu\n", strlen(pwd)) );


		crc = crc32(0, (const void *)&pwd, strlen(pwd));

		DEBUG( ("Calculated crc32: %u\n", crc) );
	}
	else
	{
		DEBUG( ("Taking password from cmd-param, length: %lu\n", strlen(password)) );

		crc = crc32(0, (const void *)password, strlen(password));

		DEBUG( ("Calculated crc32: %u\n", crc) );
	}

	return crc;
}


void initRandom(unsigned int seed)
{
	srand(seed);

	DEBUG( ("Pseudo Number Generator initialized with: %u\n", seed) );
}


unsigned int getNextRandom()
{
	return rand();
}


