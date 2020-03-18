/*************************************************************************\
 * File Name	: lists.h			      		         *
 * --------------------------------------------------------------------- *
 * Title	: EXOS --- Event-Driven system                           *
 * Revision	: V1.0                                                   *
 * Notes	:                                                        *
 * --------------------------------------------------------------------- *
 * Revision History:                                                     *
 *   When	      Who	  Revision	 Description of change   *
 * -----------	  -----------	 ---------      ------------------------ *
 * 20-09-2010	  charlie_weng	   V1.0	         Created the program     *
 *                                                                       * 
\*************************************************************************/

#ifndef _LISTS_H_
#define _LISTS_H_

#include "EXOS.h"

////////////////////////////////////////////////////////////////////////// 

#define isempty(q) ((q) == NULL)
//////////////////////////////////////////////////////////////////////////

void   put(Thread t, Thread *q) ;
Thread get(Thread *q);
void   delete(Thread t, Thread *q);

#endif 


