/*
 * fileop.c
 *
 *  Created on: May 7, 2014
 *      Author: Sebastian Rosenberg
 */

#include <stdio.h>

void initFile(FILE *file)
{
	rewind(file);
}

void writeByteToFile(FILE *file, char byte)
{
	fputc(byte, file);
}


char getByteFromFile(FILE *file)
{
	char c;

	if( EOF == (c = fgetc(file)) )
	{
		return EOF;
	}
	else
	{
		return c;
	}
}

int getFileLength(FILE *file)
{
	int current_post = 0;
	int length = 0;

	current_post = ftell(file);
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	fseek(file, current_post, SEEK_SET);

	return length;

}
