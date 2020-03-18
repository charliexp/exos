/*************************************************************************\
 * File Name	: MSG.C	      		       		         	 *
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

#include <string.h>
#include "EXOS.H"
#include "EXOSSEM.h"
#include "MSGQUE.H"

//////////////////////////////////////////////////////////////////////////


/*************************************************************************\
 *                                                                       *
 *   Function name   : eXosMsgBoxInit                                    *
 *   Returns 	     : void                                              *
 *   Parameters      : MsgBox                                            *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void  eXosMsgQueInit(MsgQue c)
{
	eXosSemInit(&c->send, 1);
	eXosSemInit(&c->recv, 0);
	eXosSemInit(&c->sync, 0);
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : eXosMsgBoxSend                                    *
 *   Returns 	     : void                                              *
 *   Parameters      : MsgBox, void* ,int                                *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
int  eXosMsgQueSend(MsgQue c, void *ptr, int size)
{
	assert(c);
	assert(ptr);
	assert(size >= 0);
	
	eXosSemWait(&c->send);
	
	c->ptr  =  ptr;
	c->size = &size;
	
	eXosSemSignal(&c->recv);
	eXosSemWait(&c->sync);
	
	return size;	
}
/*************************************************************************\
 *                                                                       *
 *   Function name   : eXosMsgBoxReceive                                 *
 *   Returns 	     : void                                              *
 *   Parameters      : MsgBox,void*,int                                  *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
int  eXosMsgQueReceive(MsgQue c, void *ptr, int size)
{
	int n;
	
	assert(c);
	assert(ptr);
	assert(size >= 0);
	
	eXosSemWait(&c->recv);
	
	n = *c->size;
	if (size < n){
		n = size;
	}
	*c->size = n;
	if (n > 0){
		memcpy(ptr, c->ptr, n);
	}
	
	eXosSemSignal(&c->sync);
	eXosSemSignal(&c->send);
	
	return n;
}

///////////////////////////////////////////////////////////////////////////

