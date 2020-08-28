#ifndef __QUEUE_H
#define __QUEUE_H

#define QUEUE_INT_DISABLE        SCH_INT_DISABLE
#define QUEUE_INT_ENABLE        SCH_INT_ENABLE

typedef SCH_BOOL		QUE_BOOL;    /* Bool */
typedef SCH_S8			QUE_S8;        /* signed char */
typedef SCH_U8			QUE_U8;        /* unsigned char */
typedef SCH_S16			QUE_S16;        /* signed short */
typedef SCH_U16			QUE_U16;        /* unsigned short */
typedef SCH_S32			QUE_S32;        /* signed int */
typedef SCH_U32			QUE_U32;        /* unsigned int */

///=========================
typedef struct
{
	QUE_U16 Cnt;            ///
	QUE_U16 Head;            ///
	QUE_U16 Tail;            ///
	const QUE_U16 Qsize;        ///
	const QUE_U16 Isize;        ///
	void *const Pbuf;            ///
}QUEUE_T;

extern void Queue_Init(QUEUE_T *p_queue);
extern QUE_U16 Queue_Cnt(QUEUE_T *p_queue);
extern QUE_BOOL Queue_IsFull(QUEUE_T *p_queue);
extern QUE_BOOL Queue_IsEmpty(QUEUE_T *p_queue);
extern QUE_BOOL Queue_In(QUEUE_T *p_queue,void *data,QUE_U16 Len);
extern QUE_BOOL Queue_Out(QUEUE_T *p_queue, void *data, QUE_U16 Len);
extern QUE_BOOL Queue_Insert(QUEUE_T *p_queue, void *p_data, QUE_U16 Len);
#endif
