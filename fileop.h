/*
 * fileop.h
 *
 *  Created on: May 7, 2014
 *      Author: Sebastian Rosenberg
 */

#ifndef FILEOP_H_
#define FILEOP_H_

void initFile(FILE *file);
void writeByteToFile(FILE *file, char byte);
char getByteFromFile(FILE *file);
int getFileLength(FILE *file);

#endif /* FILEOP_H_ */
