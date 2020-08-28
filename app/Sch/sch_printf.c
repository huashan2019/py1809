/*
***************************************************************************************************
**  Copyright   : Copyright(c) 2015 PROTRULY. All rights reserved.
**  Project     : KEA128_Frame
**  File        : printf.c
**  Description : This file is used to provide print function.
**  Author      : YizhongLiang
**  Created on  : 2015.07.23 - 2015.07.23
**  Note        : NULL
***************************************************************************************************
*/
#include "sch_config.h"
#ifdef PRINTF_EN

/*
*****************************************************
*/
const PRT_U8 *const p_Version_printf = "V0.1.0.0723";                                              /* �汾 */


static void Print_To_Buf(PRT_U8 *data)
{
	while(!PRINT_TO_TXBUF(data));
}
/*
*******************************************************************************
**  ����  : Printf_Conversion
**  ����  : yizhongliang(yizhong_1986@163.com)
**  ����  : 2015.06.20 - 2015.07.23
**  ����  : �������������ʵ�ִ�ӡ���ܵĸ�ʽת����
**  ����  : - num   : �ɱ�Ĳ�����
**        - str   : ת����洢�ĵ�ַ��
**        - radix : Ҫ���к��ָ���ת����
**  ����  : ��
**  ��ע  : ��
*********************************************************************************
*/
static void __Printf_Conversion (PRT_U32 num, PRT_U8 *str, PRT_U8 radix)
{
	PRT_U32 i;                                                                                    /* ����ѭ�� */
	PRT_U32 j;                                                                                    /* ����ѭ�� */
	PRT_U8 *ptr = str;                                                                            /* �洢ת����ĵ�ַ */
	PRT_U8 String[] = "0123456789ABCDEF";                                                        /* ��������ת�����ַ��� */
	if (num == 0)                                                                                  /* �����0�Ͷಹ����0 */
	{
		*ptr++ = '0';                                                                              /* ��0 */
		*ptr = '\0';                                                                             /* �������� */
	}
	else                                                                                           /* ��0 */
	{
		while (num)                                                                                /* Num > 0 */
		{
			if (num < radix)                                                                       /* �ж��Ƿ�Ҫ����ת�� */
			{
				*ptr++ = String[num];                                                              /* ʮ������ת�� */
				if (radix == PRINTF_CONTOHEX)                                                      /* ʮ������ת�� */
				{
					*ptr++ = 'x';                                                                  /* ʮ��������ǰ��0x */
					*ptr++ = '0';                                                                  /* ʮ��������ǰ��0x */
				}
				*ptr = '\0';                                                                     /* �������� */
				break;                                                                             /* ���� */
			}
			*ptr++ = String[num % radix];                                                        /* ȡ��λ */
			num /= radix;                                                                      /* ȡ��λ */
		}
	}
	j = ptr - str - 1;                                                                             /* �ж��Ƿ�Ҫ����λ�ý��� */
	if (j > 0)                                                                                     /* �ж��Ƿ�Ҫ����λ�ý��� */
	{
		char Temp;                                                                                 /* ��ʱ���� */
		for (i = 0; i < ((ptr - str) / 2); i++)                                                    /* ����ǰ��λ�ý��� */
		{
			Temp = *(str + i);                                                           /* ����ǰ��λ�ý��� */
			*(str + i) = *(str + j - i);                                                       /* ����ǰ��λ�ý��� */
			*(str + j - i) = Temp;                                                                 /* ����ǰ��λ�ý��� */
		}
	}
}
/*
*******************************************************************************
**  ����  : Debug_printf
**  ����  : yizhongliang(yizhong_1986@163.com)
**  ����  : 2015.06.15 - 2015.07.23
**  ����  : �������������ʵ�ִ�ӡ���ܵġ�
**  ����  : - fmt : �ɱ�Ĳ�����
**  ����  : ��
**  ��ע  : ��
*********************************************************************************
*/
void Printf (PRT_U8 *fmt, ...)
{
	PRT_U8  *str;                                                                           /* �����洢��ȡ���Ĳ��� */
	PRT_U8  ConBuf[16] = {0};                                                               /* �����洢���и�ʽת������ʱ���� */
	va_list ap;                                                                                    /* ����һ��ָ�� */
	va_start (ap, fmt);                                                                            /* ��ʼ��ȡ���� */
	while (*fmt)                                                                                   /* ֱ�����Ϊֹ */
	{
		if (*fmt != '%')                                                                           /* �ж��ǲ����и�ʽ��� */
		{
			Print_To_Buf(fmt++);
			continue;                                                                              /* ���� */
		}
		switch (*++fmt)                                                                            /* �ж�Ҫ���к��ָ�ʽ��� */
		{
			case 's' :                                                                             /* �ַ��� */
			case 'S' :
				str = va_arg (ap, PRT_U8*);                                                  /*  */
				for ( ; *str; str++)                                                               /* ������ */
					Print_To_Buf(str);
				break;                                                                             /* ���� */
			case 'd' :                                                                             /* ʮ���� */
			case 'D' :
				__Printf_Conversion (va_arg(ap, PRT_U32), ConBuf, PRINTF_CONTODEC);                /* ���и�ʽת�� */
				for (str = ConBuf; *str; str++)                                                    /* ������ */
					Print_To_Buf(str);
				break;                                                                             /* ���� */
			case 'x' :                                                                             /* ʮ������ */
			case 'X' :
				__Printf_Conversion (va_arg (ap, PRT_U32), ConBuf, PRINTF_CONTOHEX);               /* ���и�ʽת�� */
				for (str = ConBuf; *str; str++)                                                    /* ������ */
					Print_To_Buf(str);
				break;                                                                             /* ���� */
			case '%':
				Print_To_Buf(fmt);
				break;
			default :                                                                              /* ����δ�������� */
				Print_To_Buf(str);
				break;                                                                             /* ���� */
		}
	fmt++;                                                                                     /* ��һ������ */
	}
	va_end (ap);                                                                                   /* ֹͣѰ�Ҳ������Ҳµİ� */
}
#endif
/* ************************************ File end *********************************************** */
