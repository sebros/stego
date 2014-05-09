/*
 * quicksort.c
 *
 *  Created on: May 6, 2014
 *      Author: Sebastian Rosenberg
 */

#include <stdlib.h>
#include "types.h"
#include "quicksort.h"




int quickSortCompare (const void * a, const void * b)
{

  PixelMap *pixelA = (PixelMap *)a;
  PixelMap *pixelB = (PixelMap *)b;

  if( pixelA->value > pixelB->value )
  {
	  return -1;
  }
  if( pixelA->value < pixelB->value )
  {
	  return 1;
  }
  if( (pixelA->value == pixelB->value) && (pixelA->index > pixelB->index) )
  {
	  return -1;
  }
  if( (pixelA->value == pixelB->value) && (pixelA->index < pixelB->index) )
  {
	  return 1;
  }
  else return 0;	//should never be reached, indexes are never the same
}




void quickSortPixelMap(PixelMap *pixelMap, long unsigned int size)
{
	qsort(pixelMap, size, sizeof(PixelMap), quickSortCompare);
}
