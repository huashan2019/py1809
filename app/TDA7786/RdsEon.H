/********************************************************************************\
*	Car audio RDS head file												*
*             	Language           : C-Like.				     				*
* Author:  SAMWELL TECHNICAL Co.,LTD                                      		*
\*------------------------------------------------------------------------------*/

#ifdef   RDSEon_ROOT
#define  RDSEon_EXT
#else
#define  RDSEon_EXT     extern
#endif

#define _RDSEonLength	  	8
#define	_EONStructSize		8
#define _EonAfLength		25             /* .*/

typedef struct
{
  uint Pi;					/* EON PI code .*/
  uchar Af[_EonAfLength];	/* EON Alternative frequency by cross ref.*/ 
  uchar Flag;				/* Bad EON station counter.*/
} EonData;

RDSEon_EXT EonData	RDSEonBlkData[_EONStructSize];

RDSEon_EXT	uchar  	rRdsEonVar;				/* Dynamic EON data: variante.*/
RDSEon_EXT	uchar  	rRdsEonData1;			/* Dynamic EON data: Block C high byte.*/
RDSEon_EXT	uchar  	rRdsEonData2;			/* Dynamic EON data: Block C low  byte.*/

RDSEon_EXT	uchar  	rRdsEonTaSt;			/* EON TA management actual state.*/
#define _RdsEonTaWait   0					/* EON TA waitting state.*/
#define _RdsEonTaReq    1					/* EON TA active request state.*/
#define _RdsEonTaActIf  2					/* EON TA active state.*/
#define _RdsEonPiChk	3					/* EON TA active state.*/
#define _RdsEonChkSt  	4					/* EON TA active state.*/
#define _RdsEonTaOn     5					/* EON TA on mode state.*/
#define _RdsEonTaEnd	6					/* EON TA on mode state.*/

RDSEon_EXT	uchar  	rRdsEonTaIdx;			/* EON TA management EON structure index.*/  

RDSEon_EXT	uchar  	rRdsEonPtySt;			/* EON PTY management actual state. */
#define _RdsEonPtyWait	0							/* EON PTY waiting state. */
#define _RdsEonPtyOn	1							/* EON PTY on mode state.*/ 

RDSEon_EXT	uchar  	rRdsEonPtyIdx;			/* EON PTY management EON structure index.*/

RDSEon_EXT	uchar  	rRdsEon_X64ms;			/* EON PTY management EON structure index.*/

RDSEon_EXT	uchar  	rRdsEonIdx;				/* Rolling EON structure index.*/

//RDSEon_EXT	TYPE_WORD  	cOldRadStatNum;		/* Backup old station.*/
//#define	cOldRadStatL	cOldRadStatNum.byte[0]		/* Backup old station.*/
//#define	cOldRadStatH	cOldRadStatNum.byte[1]		/* Backup old station.*/

RDSEon_EXT	TYPE_BYTE 	rRdsEonFlag;
#define	bEonAfDet		rRdsEonFlag.bit.b0
#define	bMsgEndEonTa	rRdsEonFlag.bit.b1



extern	void	RDSEonAfInit(uchar eonidx);
extern	void 	RDSEonAfInsert (uchar reonidx, uchar reonfreq);


