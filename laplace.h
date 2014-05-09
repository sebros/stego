/*
 * laplace.h
 *
 *  Created on: May 6, 2014
 *      Author: Sebastian Rosenberg
 */

#ifndef LAPLACE_H_
#define LAPLACE_H_

#include <opencv2/core/core_c.h>
#include <opencv2/imgproc/imgproc_c.h>

#include "types.h"

int getLaplacePixelMap(IplImage *image, PixelMap **pixelMap, int lsb);

#endif /* LAPLACE_H_ */
