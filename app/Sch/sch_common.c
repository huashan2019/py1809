/*
***************************************************************************************************
**  Copyright   : 
**  Project     : STM32F103
**  File        : sch_common.c
**  Description : This file is the SCH.
**  Author      : WenjunHu
**  Created on  : 2017.06.30
**  Note        : NULL
***************************************************************************************************
*/
#include "sch_config.h"



/*************************************************************************************
**  Function    : SysWaitXXX
**  Author		: wenjunHu
**  Created on	: 
**  Description	: 等待时间 Delay
**  Return		: NULL
**************************************************************************************/
void SysWait1Us(void)
{
#if 1
	SCH_U8 index = 2;
	while(index--)
		SCH_NOP;
	SCH_NOP;SCH_NOP;SCH_NOP;
	SCH_NOP;SCH_NOP;
#else
	SHC_NOP;SHC_NOP;SHC_NOP;
	SHC_NOP;SHC_NOP;SHC_NOP;
	SHC_NOP;SHC_NOP;SHC_NOP;
	SHC_NOP;SHC_NOP;SHC_NOP;
	SHC_NOP;SHC_NOP;SHC_NOP;
	SHC_NOP;SHC_NOP;SHC_NOP;
	SHC_NOP;SHC_NOP;SHC_NOP;
	SHC_NOP;SHC_NOP;SHC_NOP;
	SHC_NOP;SHC_NOP;SHC_NOP;
#endif
}
void SysWaitUs(SCH_U32 Time)
{
	while(Time--)
		SysWait1Us();
}
void SysWaitMs(SCH_U32 Time)
{
	while(Time--)
		SysWaitUs(1000);
}


/*************************************************************************************
**  Function    : GetMax GetMin
**  Author		: wenjunHu
**  Created on	: 20170608
**  Description	: 比较大小函数
**  Return		: NULL
**************************************************************************************/
SCH_U32 GetMax(SCH_U32 m,SCH_U32 n)
{
	return (m>n)?m:n;
}
SCH_U32 GetMin(SCH_U32 m,SCH_U32 n)
{
	return (m>n)?n:m;
}
/*************************************************************************************
**  Function    : sch_memxxx
**  Author		: wenjunHu
**  Created on	: 20170608
**  Description	: 拷贝函数
**  Return		: NULL
**************************************************************************************/
void sch_memset(void *s,const SCH_U8 c,SCH_U16 n)  
{
    SCH_U8 *su = (SCH_U8 *)s;
	while(n--)
	{
		*su++ = c;
	}
}  
void sch_memcpy(void *to,  void const *from, SCH_U16 n)  
{
	SCH_U8 *t = (SCH_U8 *)to;
	SCH_U8 *f = (SCH_U8 *)from;
	while(n--)
	{
		*t++ = *f++;
	}
}  
/*************************************************************************************
**  Function    : GetCheckData 
**  Author		: wenjunHu
**  Created on	: 20170608
**  Description	: 获取校验码
**  Return		: NULL
**************************************************************************************/
SCH_U8 GetCheckData_Add(SCH_U8 *data, SCH_U8 Len)
{
	SCH_U8 index=0;
	while(Len--)
		index += *data++;
	return index;
}
SCH_U8 GetCheckData_Xor(SCH_U8 *data, SCH_U8 Len)
{
	SCH_U8 index=0;
	while(Len--)
		index ^= *data++;
	return index;
}
/*************************************************************************************
**  Function    :  IsInXXX
**  Author		: wenjunHu
**  Created on	: 20170703
**  Description	: 判断一个整数是否在某个范围内
**  Return		: NULL
**************************************************************************************/
SCH_BOOL IsInRange(SCH_U32 min,SCH_U32 data,SCH_U32 max)
{
	if(min > max)
		return FALSE;
	if(data >= min && data <= max)
		return TRUE;
	return FALSE;
}
SCH_BOOL IsInData(SCH_U32 data0,SCH_U32 data1,SCH_U32 step)
{
	SCH_U32 index;
	index = (data0 > data1) ? (data0 - data1) : (data1 - data0);
	if(index <= step)
		return TRUE;
	return FALSE;
}
/*************************************************************************************
**  Function    : GetAverage_UXX
**  Author		: wenjunHu
**  Created on	: 20170803
**  Description	: 求一个数组的平均值
**  Return		: NULL
**************************************************************************************/
SCH_U16 GetAverage_U16(SCH_U16 *pTable, SCH_U8 Array_Cnt)
{
	SCH_U32 Data = 0x00000000;
	SCH_U8 index;
	for(index=0;index<Array_Cnt;index++)
	{
		Data += pTable[index];
	}
	return (SCH_U16)(Data/Array_Cnt);
}
SCH_U8 GetAverage_U8(SCH_U8 *pTable, SCH_U8 Array_Cnt)
{
	SCH_U16 Data = 0x0000;
	SCH_U8 index;
	for(index=0;index<Array_Cnt;index++)
	{
		Data += pTable[index];
	}
	return (SCH_U8)(Data/Array_Cnt);
}
/*************************************************************************************
**  Function    : IsInArray_UXX
**  Author		: wenjunHu
**  Created on	: 20170803
**  Description	: 求一个数在数组的位置(下标)
**  Return		: NULL
**************************************************************************************/
SCH_BOOL IsInArray_U8(SCH_U8 Data,SCH_U8 *pTable,SCH_U8 Array_Cnt,SCH_U8 *num)
{
	SCH_U8 index;
	for(index=0;index<Array_Cnt;index++)
	{
		if(pTable[index] == Data)
		{
			*num = index;
			return TRUE;
		}
	}
	return FALSE;
}
SCH_BOOL IsInArray_U16(SCH_U16 Data,SCH_U16 *pTable,SCH_U8 Array_Cnt,SCH_U8 *num)
{
	SCH_U8 index;
	for(index=0;index<Array_Cnt;index++)
	{
		if(pTable[index] == Data)
		{
			*num = index;
			return TRUE;
		}
	}
	return FALSE;
}













