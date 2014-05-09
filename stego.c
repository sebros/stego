/*
 * stego.c
 *
 *  Created on: May 2, 2014
 *      Author: Sebastian Rosenberg
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "types.h"
#include "debug.h"
#include "random.h"
#include "quicksort.h"
#include "laplace.h"
#include "fileop.h"


#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc_c.h>


void printHelp()
{
	puts("stego - (C) Sebastian Rosenberg - 2014");
	puts("");
	puts("Command-line parameters:");
	puts("-i <file>      integrate message to file");
	puts("-e <file>      extract message from file");
	puts("-m <file>      message file");
	puts("-o <file>      file to write");
	puts("-p <password>  set password from command-line");
	//puts("-l <lsb_bit>   how many lsb bit's to be used [1,2]");
	puts("-t <treshold>  treshold calculating the bits to be taken [1-20] default: 10");
	puts("-c             only calculate values and print them");
	puts("");
	puts("-h / --help     show this help");
}

short int charToNum(char c)
{
	return c-48;
}


void getCmdLineParamsToConfig(int argc, char **argv, ConfigInfo *configinfo)
{
	char c;

    while ((c = getopt (argc, argv, "i:e:m:o:p:l:t:ch")) != -1)
    {
    	switch (c)
    	{
    	case 'i':
    		configinfo->cover_file = optarg;
    		break;
    	case 'e':
    		configinfo->stego_file = optarg;
    		break;
    	case 'm':
    		configinfo->message_file = optarg;
    		break;
        case 'o':
        	configinfo->output_file = optarg;
        	break;
        case 'p':
        	configinfo->password = optarg;
        	break;
        case 'l':
        	if( 0 == strcmp(optarg, "1") ) { configinfo->lsb_usage = LSB_USAGE_1; }
        	else if( 0 == strcmp(optarg, "2") ) { configinfo->lsb_usage = LSB_USAGE_2; }
        	else { configinfo->lsb_usage = LSB_USAGE_1; }
        	break;
        case 't':
    		configinfo->treshold = DEFAULT_TRESHOLD;

        	if( 2 == strlen(optarg) )
        	{
        		if( 2 == charToNum(optarg[0]) )
        		{
        			configinfo->treshold = 20;
        		}
        		else if ( 1 == charToNum(optarg[0]) )
        		{
        			if( (0<=charToNum(optarg[1])) && (10>charToNum(optarg[1])) )
        			{
        				configinfo->treshold += charToNum(optarg[1]);
        			}
        		}
        	}
        	else if (1 == strlen(optarg))
        	{
    			if( (0<=charToNum(optarg[0])) && (10>charToNum(optarg[0])) )
    			{
    				configinfo->treshold = charToNum(optarg[0]);
    			}
        	}

        	break;
        case 'c':
        	configinfo->only_calc = TRUE;
        	break;
        case 'h':
        	printHelp();
        	exit(0);
        	break;
        case '?':
        	printHelp();
        	exit(1);
        	break;
        default:
        	printHelp();
        	exit(1);
        	break;
        }
    }

    if( (configinfo->cover_file != NULL) && (configinfo->stego_file != NULL) && (configinfo->message_file == NULL) && (configinfo->output_file == NULL) )
    {
    	if(configinfo->only_calc == TRUE) return;
    	printHelp();
    	exit(1);
    }

    if( (configinfo->stego_file != NULL) && (configinfo->cover_file != NULL) && (configinfo->message_file == NULL) )
    {
    	if(configinfo->only_calc == TRUE) return;
    	printHelp();
    	exit(1);
    }

    if( (configinfo->cover_file == NULL) && (configinfo->stego_file == NULL) )
    {
    	printHelp();
    	exit(1);
    }

}


int main(int argc, char **argv) {

	ConfigInfo configinfo;

	IplImage *img;
	char *img_path;
	int img_channels;

	PixelMap *pixelmap;
	int pixelmap_iterations;
	unsigned long int area_size;

	unsigned int treshold;
	unsigned int useable_pixels;

	FILE *file;

	unsigned int seed;


	//first set struct to 0
	memset(&configinfo, 0, sizeof(ConfigInfo));

	configinfo.treshold = DEFAULT_TRESHOLD;

	getCmdLineParamsToConfig(argc, argv, &configinfo);

	configinfo.lsb_usage = LSB_USAGE_1; //TODO: fix to userinput



	if(configinfo.cover_file != NULL)
	{
		img_path = configinfo.cover_file;
	}

	if(configinfo.stego_file != NULL)
	{
		img_path = configinfo.stego_file;
	}


	img = cvLoadImage(img_path, CV_LOAD_IMAGE_COLOR); //TODO: load image with original channel depth
	if(!img) {
		printf("Error: Failed to Load Image %s\n", img_path);
		exit(1);
	}

	area_size = img->width * img->height;


	if( 0 == (pixelmap_iterations = getLaplacePixelMap(img, &pixelmap, configinfo.lsb_usage)) )
	{
		printf("Error: can't extract laplace-information\n");
		cvReleaseImage( &img );
		exit(1);
	}

	quickSortPixelMap(pixelmap, area_size);

	//calculate treshold, factor 100 is a good value here
	treshold = (unsigned int) pixelmap[ area_size/(100-configinfo.treshold) ].value;


	//count number of pixels to use
	for(useable_pixels=0; ;useable_pixels++)
	{
		if( pixelmap[useable_pixels].value < treshold ) break;
	}


	if(img->nChannels == 1)
	{
		img_channels = 1;
	}
	if(img->nChannels >= 3){
		img_channels = 3;
	}


	if( configinfo.only_calc == TRUE)
	{
		printf("usable data size to embedd is %u Bytes\n", (unsigned int)( ((useable_pixels * img_channels) - MESSAGE_LENGTH_BITS) / BYTE ));	//32 bits for message length

		cvReleaseImage( &img );
		free( pixelmap );
		exit(0);
	}


	seed = generateSeed(configinfo.password);
	initRandom(seed);

	//write data to cover
	if(configinfo.cover_file != NULL)
	{

		if( NULL == (file = fopen(configinfo.message_file, "r")) )
		{
			printf("Failed to open message file: %s\n", configinfo.message_file);

			cvReleaseImage( &img );
			free( pixelmap );
			exit(0);
		}

		writeDataToImage(img, pixelmap, file, configinfo, useable_pixels); //TODO: catch return value

		cvSaveImage(configinfo.output_file, img, NULL);

	}
	//extract data from stego file
	else if(configinfo.stego_file != NULL)
	{
		if( NULL == (file = fopen(configinfo.message_file, "w")) )
		{
			printf("Failed to open message file: %s\n", configinfo.message_file);

			cvReleaseImage( &img );
			free( pixelmap );
			exit(0);
		}

		readDataFromImage(img, pixelmap, file, configinfo, useable_pixels); //TODO: catch return value
	}
	else
	{
		//this should not be reached
		DEBUG( ("Something went totally wrong! line: %d\n", __LINE__) );
	}


	cvReleaseImage( &img );
	free( pixelmap );
	fclose(file);

	return 0;

}




