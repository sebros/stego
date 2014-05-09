/*
 * laplace.c
 *
 *  Created on: May 6, 2014
 *      Author: Sebastian Rosenberg
 */

#include "types.h"
#include "debug.h"

#include "laplace.h"

#include <stdio.h>

#include <opencv2/core/core_c.h>
#include <opencv2/imgproc/imgproc_c.h>

int getLaplacePixelMap(IplImage *image, PixelMap **pixelMap, int lsb)
{
	IplImage *lsb_image;
	IplImage *grayscale_image;
	IplImage *laplace_image;
	CvScalar pixel;

	int laplace_depth = IPL_DEPTH_16S;
	int lsb_number = 255 - lsb;
	int memory_size=0;
	int i,j, k, index=0;


	if(NULL == image)
	{
		DEBUG( ("Image Null-Pointer in getLaplacePixelMap(): %d\n", __LINE__) );
		return 0;
	}

	//check for image depth and fix it to laplace operator
	//	((src.depth() == CV_8U && (dst.depth() == CV_16S || dst.depth() == CV_32F)) ||
	//	(src.depth() == CV_32F && dst.depth() == CV_32F))
	if( image->depth == IPL_DEPTH_32F )
	{
		laplace_depth = IPL_DEPTH_32F;
	}

	if( NULL == (lsb_image = cvCloneImage(image) ) )
	{
		DEBUG( ("Failed to clone original image in getLaplacePixelMap(): %d\n", __LINE__) );
		return 0;
	}

	if( NULL == (grayscale_image = cvCreateImage( cvSize(lsb_image->width, lsb_image->height), IPL_DEPTH_8U, 1)) )
	{
		DEBUG( ("Failed to create grayscale-image for laplace-operator in getLaplacePixelMap(): %d\n", __LINE__) );
		cvReleaseImage( &lsb_image );
		return 0;
	}

	if( NULL == (laplace_image = cvCreateImage( cvSize(grayscale_image->width, grayscale_image->height), laplace_depth, 1 )) )
	{
		DEBUG( ("Failed to create image map for laplace-operator in getLaplacePixelMap(): %d\n", __LINE__) );
		cvReleaseImage( &lsb_image );
		cvReleaseImage( &grayscale_image );
		return 0;
	}


	//cut lsb bits
	for(i=0; i<lsb_image->height; i++)
	{
		for(j=0; j<lsb_image->width; j++)
		{
			for(k=0; k<lsb_image->nChannels; k++)
			{
				lsb_image->imageData[i*lsb_image->widthStep + j* lsb_image->nChannels + k]
				                     = lsb_image->imageData[i*lsb_image->widthStep + j* lsb_image->nChannels + k]
				                       & lsb_number;
			}
		}
	}

	cvCvtColor(lsb_image, grayscale_image, CV_RGB2GRAY);

	cvReleaseImage( &lsb_image );

	printf("Calculating Laplace Operator...");

	cvLaplace(grayscale_image, laplace_image, 3);	//TODO: give commandline option to change this param

	printf("done\n");

	cvReleaseImage( &grayscale_image );


	memory_size = (sizeof(PixelMap) * laplace_image->width * laplace_image->height);

	*pixelMap = malloc( memory_size );

	if(NULL == *pixelMap)
	{
		cvReleaseImage( &laplace_image );
		DEBUG( ("Failed to allocate pixelMap memory: %d Bytes\n", memory_size) );
	}
	else
	{
		DEBUG( ("%d Bytes for pixelMap allocated\n", memory_size) );
	}


	printf("getting image map...");

	for (i = 0; i < laplace_image->height; i++) {
	   for (j = 0; j < laplace_image->width; j++) {
	      pixel = cvGet2D(laplace_image, i, j);

		  (*pixelMap)[i*laplace_image->width + j].index = index++;
		  (*pixelMap)[i*laplace_image->width + j].value = (int)pixel.val[0];
		  (*pixelMap)[i*laplace_image->width + j].used = FALSE;
		  (*pixelMap)[i*laplace_image->width + j].x_coord = i;
		  (*pixelMap)[i*laplace_image->width + j].y_coord = j;

	   }
	}

	cvReleaseImage( &laplace_image );

	printf("done\n");

	DEBUG( ("%d indexes written\n", index) );

	return index;
}

