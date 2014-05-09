/*
 * types.h
 *
 *  Created on: May 7, 2014
 *      Author: Sebastian Rosenberg
 */

#ifndef TYPES_H_
#define TYPES_H_



typedef struct _ConfigInfo {

	char *cover_file;
	char *stego_file;
	char *message_file;
	char *output_file;
	char *password;
	short int only_calc;
	short int lsb_usage;
	short int treshold;

} ConfigInfo;



typedef struct _PixelMap {
	int x_coord;
	int y_coord;
	int value;			//laplace value
	int index;			//index in array
	short int used;		//determine if index already matched by prng
} PixelMap;



enum {
	LSB_USAGE_1 = 1,
	LSB_USAGE_2 = 2
};



#define MAX_PASSWORD_LEN 100
#define MIN_PASSWORD_LEN 8

#define DEFAULT_TRESHOLD 10

#define MESSAGE_LENGTH_BITS 32
#define BYTE 8
#define BIT 1

#define TRUE 1
#define FALSE 0

#endif /* TYPES_H_ */
