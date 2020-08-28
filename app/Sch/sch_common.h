#ifndef __SCH_COMMON_H
#define __SCH_COMMON_H


#define SCH_NULL  (void *) 0


#define NONE   0  

#ifdef	FALSE
#undef	FALSE
#endif
#define FALSE	(0)

#ifdef	TRUE
#undef	TRUE
#endif
#define	TRUE	(1)

#ifdef  ON
#undef  ON
#endif
#define ON      (1)

#ifdef  OFF
#undef  OFF
#endif
#define OFF     (0)


#define ERROR       1
#define NORMAL      0

#define HIGH_LEVEL  1
#define LOW_LEVEL   0

#define DOWN		0
#define UP			1

//#define  TRUE 1
//#define  FALSE 0
#define  NULL 0

#define LSB(WORD)     (SCH_U8)WORD
#define MSB(WORD)     (SCH_U8)(WORD>>8)
#define SCH_WORD(MSB,LSB)	  (SCH_U16)(((SCH_U16) MSB << 8) + LSB)

#define dim(x)         (sizeof(x)/sizeof(x[0]))

#define SetBit(VAR,Place)         ( (VAR) |= (SCH_U8)((SCH_U8)1<<(SCH_U8)(Place)) )
#define ClrBit(VAR,Place)         ( (VAR) &= (SCH_U8)((SCH_U8)((SCH_U8)1<<(SCH_U8)(Place))^(SCH_U8)255) )
#define GetBit(Var,Place)         ( (Var& (1<<(Place&0x7)))==0?0:1)


typedef struct BitChar {
	SCH_U8 B0:1;
	SCH_U8 B1:1;
	SCH_U8 B2:1;
	SCH_U8 B3:1;
	SCH_U8 B4:1;
	SCH_U8 B5:1;
	SCH_U8 B6:1;
	SCH_U8 B7:1;
} SBitChar;
#if 1
typedef union  CharField{
	SCH_U8    byte;
	SBitChar  field;
} UCharField;

typedef union  CharInt{
	unsigned char	byte[2];// byte[1]=L, byte[0]=H in hiware
	unsigned int Dbyte;
} UCharInt;


typedef union  CharLong{
	unsigned char	byte[4];// byte[0]=H
	unsigned long int Lbyte;
} UCharLong;
#endif

extern void SysWait1Us(void);
extern void SysWaitUs(SCH_U32 Time);
extern void SysWaitMs(SCH_U32 Time);
extern SCH_U32 GetMax(SCH_U32 m,SCH_U32 n);
extern SCH_U32 GetMin(SCH_U32 m,SCH_U32 n);
extern void sch_memset(void *s,SCH_U8 c,SCH_U16 n);
extern void sch_memcpy(void *to,  void const *from, SCH_U16 n);
extern SCH_U8 GetCheckData_Add(SCH_U8 *data, SCH_U8 Len);
extern SCH_U8 GetCheckData_Xor(SCH_U8 *data, SCH_U8 Len);
extern SCH_BOOL IsInRange(SCH_U32 min,SCH_U32 data,SCH_U32 max);
extern SCH_BOOL IsInData(SCH_U32 data0,SCH_U32 data1,SCH_U32 step);
extern SCH_U16 GetAverage_U16(SCH_U16 *pTable, SCH_U8 Array_Cnt);
extern SCH_U8 GetAverage_U8(SCH_U8 *pTable, SCH_U8 Array_Cnt);
extern SCH_BOOL IsInArray_U8(SCH_U8 Data,SCH_U8 *pTable,SCH_U8 Array_Cnt,SCH_U8 *num);
extern SCH_BOOL IsInArray_U16(SCH_U16 Data,SCH_U16 *pTable,SCH_U8 Array_Cnt,SCH_U8 *num);
#endif
