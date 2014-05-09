/*
 * debug.h
 *
 *  Created on: May 3, 2014
 *      Author: Sebastian Rosenberg
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#ifdef DEBUGINFORMATION
#define DEBUG(x) printf x;
#else
#define DEBUG(x) ;
#endif /* DEBUGINFORMATION */


#endif /* DEBUG_H_ */
