/*************************************************************************\
 * File Name	: EXOSSEM.h			       		      	 *
 * --------------------------------------------------------------------- *
 * Title	: EXOS --- Cooperative system scheduler                  *
 * Revision	: V1.0                                                   *
 * Notes	:                                                        *
 * --------------------------------------------------------------------- *
 * Revision History:                                                     *
 *   When	      Who	  Revision	 Description of change   *
 * -----------	  -----------	 ---------      ------------------------ *
 * 15-10-2010	  charlie_weng	   V1.0	         Created the program     *
 *                                                                       * 
\*************************************************************************/


#ifndef __XOS_SEM_H__
#define __XOS_SEM_H__

#include "EXOS.h"

//////////////////////////////////////////////////////////////////////////

typedef struct XOSSem 
{
	int   count;
	void *queue;
} XOSSem;

//////////////////////////////////////////////////////////////////////////
#define LOCK(mutex)      do { XOSSem *_lkmutex = &(mutex); \
	                      eXosSemWait(_lkmutex);
	                 
#define END_LOCK         eXosSemSignal(_yymutex); } while (0)

//////////////////////////////////////////////////////////////////////////

void eXosSemInit(XOSSem *s, int count);
void eXosSemWait(XOSSem *s);
void eXosSemSignal(XOSSem *s);
  
#endif
