/*
 * handledata.c
 *
 *  Created on: May 7, 2014
 *      Author: Sebastian Rosenberg
 */

#include <stdio.h>

#include <opencv2/core/core_c.h>

#include "types.h"
#include "debug.h"
#include "random.h"
#include "fileop.h"

#include "handledata.h"

int writeDataToImage(IplImage *img, PixelMap *pixelmap, FILE *file, ConfigInfo config, unsigned int useable_pixels)
{

	CvScalar pixel;
	int index, number;
	int i,j, written_bytes=0;
	int message_length;
	unsigned int img_capacity;
	char byte;

	int img_channels = 0;

	if(img->nChannels == 1)
	{
		img_channels = 1;
	}
	if(img->nChannels >= 3){
		img_channels = 3;
	}

	img_capacity = (unsigned int)( ((useable_pixels * img_channels) - MESSAGE_LENGTH_BITS) / BYTE );

	message_length = getFileLength(file);

	if(img_capacity < message_length)
	{
		printf("Image capacity is only: %u Byte -> message size is: %d\n", img_capacity, message_length);
		return 0;
	}

	printf("Image capacity is: %u Byte\n", img_capacity);
	printf("Message size is: %d Byte\n", message_length);


	DEBUG( ("writing message length bits to image\n") );

	for(i=MESSAGE_LENGTH_BITS-1; i>=0; )
	{

		//get next free pixel
		do
		{
			number = getNextRandom();

			index = number % useable_pixels;
		}
		while( pixelmap[index].used == TRUE );

		//get values at this place
		pixel = cvGet2D(img, pixelmap[index].x_coord, pixelmap[index].y_coord);


		//if we have more than one channel
		for(j=0; j<img_channels; j++)
		{
			pixel.val[j] = LSB_BIT( (int)pixel.val[j],  ((message_length >> i) & 1) );

			DEBUG( ("%u", ((message_length >> i) & 1)) );

			i--;

			if(i < 0)
			{
				break;
			}
		}

		cvSet2D(img, pixelmap[index].x_coord, pixelmap[index].y_coord, pixel);

		pixelmap[index].used = TRUE;

	}
	DEBUG( ("\n") );

	getNextRandom();//fix -> asynchron


	/////////////////////////////////////////////////////////////////////////////
	DEBUG( ("writing bytes to image...") );

	j=3;

	while( (EOF != (byte = getByteFromFile(file))) && (written_bytes < img_capacity) && (written_bytes < message_length) )
	{

		for(i=BYTE-1; i>=0; )
		{
			if(j>=img_channels)
			{
				j=0;

				do
				{
					number = getNextRandom();

					index = number % useable_pixels;
				}
				while( pixelmap[index].used == TRUE );

				pixel = cvGet2D(img, pixelmap[index].x_coord, pixelmap[index].y_coord);
			}
			else
			{
				j++;
			}


			while(j<img_channels)
			{
				pixel.val[j] = LSB_BIT( (int)pixel.val[j],  ((byte >> i) & 1) );

				i--;

				if(i < 0)
				{
					break;
				}

				j++;
			}

			cvSet2D(img, pixelmap[index].x_coord, pixelmap[index].y_coord, pixel);

			pixelmap[index].used = TRUE;
		}

		written_bytes++;

	}
	DEBUG( ("done\n") );


	printf("%d Bytes written to image\n", written_bytes);


	return written_bytes;
}





int readDataFromImage(IplImage *img, PixelMap *pixelmap, FILE *file, ConfigInfo config, unsigned int useable_pixels)
{
	CvScalar pixel;
	int index, number;
	int i,j;
	unsigned int message_length=0;
	unsigned int img_capacity;
	unsigned int extracted_bytes=0;
	char byte=0;

	int img_channels = 0;

	if(img->nChannels == 1)
	{
		img_channels = 1;
	}
	if(img->nChannels >= 3){
		img_channels = 3;
	}

	img_capacity = (unsigned int)( ((useable_pixels * img_channels) - MESSAGE_LENGTH_BITS) / BYTE );

	printf("Image capacity is: %u Byte\n", img_capacity);


	initFile(file);


	DEBUG( ("extracting message size bits \n") );

	for(i=0; i<=MESSAGE_LENGTH_BITS; )
	{

		//get next free pixel
		do
		{
			number = getNextRandom();

			index = number % useable_pixels;
		}
		while( pixelmap[index].used == TRUE );

		//get values at this place
		pixel = cvGet2D(img, pixelmap[index].x_coord, pixelmap[index].y_coord);

		//if we have more than one channel
		for(j=0; j<img_channels; j++)
		{
			message_length |= (int)pixel.val[j] & 1;

			i++;

			if(i < MESSAGE_LENGTH_BITS)
			{
				message_length <<= 1;
			}
			else
			{
				break;
			}

			DEBUG( ("%u", ((int)pixel.val[j] & 1) ) );
		}

		pixelmap[index].used = TRUE;

	}
	DEBUG( ("\n") );

	if(message_length > img_capacity)
	{
		printf("The embedded message size (%d) does not fix to image's capacity (%d)\n", message_length, img_capacity);
		printf("Please check if you have chosen the correct parameters and password\n");
		return 0;
	}

	printf("Integrated message size is: %d Byte\n", message_length);


	////////////////////////////////////////////////////////////////////////////////////////
	DEBUG( ("extracting bytes out of image...") );

	j=3;

	while( extracted_bytes < message_length )
	{
		byte=0;

		for(i=0; i<BYTE; )
		{

			if(j>=img_channels)
			{
				j=0;

				do
				{
					number = getNextRandom();

					index = number % useable_pixels;
				}
				while( pixelmap[index].used == TRUE );

				pixel = cvGet2D(img, pixelmap[index].x_coord, pixelmap[index].y_coord);
			}
			else
			{
				j++;
			}


			while(j<img_channels)
			{
				byte |= (int)pixel.val[j] & 1;

				i++;

				if(i < BYTE)
				{
					byte <<= 1;
				}
				else
				{
					break;
				}

				j++;
			}

			pixelmap[index].used = TRUE;
		}

		writeByteToFile(file, byte);

		extracted_bytes++;

	}
	DEBUG( ("done\n") );


	printf("%d Bytes extracted from image\n", extracted_bytes);


	return extracted_bytes;
}


