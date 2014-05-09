/*
 * quicksort.h
 *
 *  Created on: May 6, 2014
 *      Author: Sebastian Rosenberg
 */

#ifndef QUICKSORT_H_
#define QUICKSORT_H_

#include "types.h"

int quickSortCompare (const void * a, const void * b);
void quickSortPixelMap(PixelMap *pixelMap, long unsigned int size);

#endif /* QUICKSORT_H_ */
