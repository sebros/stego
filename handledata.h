/*
 * handledata.h
 *
 *  Created on: May 7, 2014
 *      Author: Sebastian Rosenberg
 */

#ifndef HANDLEDATA_H_
#define HANDLEDATA_H_

#define LSB_1_MASK 1
#define LSB_0_MASK -2		//-2 on two's complement is 11111110

#define LSB_BIT(num,bit) ( bit ? (num | LSB_1_MASK) : (num & LSB_0_MASK))


int writeDataToImage(IplImage *img, PixelMap *pixelmap, FILE *file, ConfigInfo config, unsigned int useable_pixels);
int readDataFromImage(IplImage *img, PixelMap *pixelmap, FILE *file, ConfigInfo config, unsigned int useable_pixels);


#endif /* HANDLEDATA_H_ */
