/*************************************************************************\
 * File Name	: MSG.h	      		       		         	 *
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

#ifndef __MSG_Q__
#define __MSG_Q__

//////////////////////////////////////////////////////////////////////////
struct MsgQue 
{
	void   *ptr;
	int   *size;
	XOSSem send;
	XOSSem recv;
	XOSSem sync;
};

//////////////////////////////////////////////////////////////////////////
typedef struct MsgQue *MsgQue;

//////////////////////////////////////////////////////////////////////////
void  eXosMsgQueInit(MsgQue c);
int   eXosMsgQueSend(MsgQue c, void *ptr, int size);
int   eXosMsgQueReceive(MsgQue c, void *ptr, int size);

//////////////////////////////////////////////////////////////////////////
#endif

