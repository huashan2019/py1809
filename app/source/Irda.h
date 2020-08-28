#ifdef     Irda_ROOT
#define  Irda_ROOT
#else
#define  Irda_ROOT     extern
#endif


Irda_ROOT	void IrdaInit(void);
//Irda_ROOT  	void IrdaIntrProbe(void);
Irda_ROOT	uchar rIrdRecSt;

enum {CODE_NEW=0, CODE_REPEAT};
typedef uint8_t (*pfnBreakWait)();
typedef enum _IRDA_TYPE
{
    IR_TYPE_1 = 0,
	IR_TYPE_2 = 1,
	IR_TYPE_3 = 2,
} IRDA_TYPE, *PIRDA_TYPE;




/*remote variable define*/
//Irda_ROOT	unsigned char	Rmt_X64ms;
Irda_ROOT	unsigned char	Rmt_X4ms;
Irda_ROOT	unsigned int	Testtime_x10ms;

Irda_ROOT	unsigned int	Testtime_x100ms;

#define		Rmt_X64ms		ks_X64ms
//#define		Rmt_X4ms		ks_X4ms


Irda_ROOT	unsigned int	Rmt_time_cnt;

Irda_ROOT	unsigned char	cRmtDataBits;
Irda_ROOT	unsigned char	rRmtSt;
Irda_ROOT	unsigned int	RmtKyBak;
Irda_ROOT	unsigned char	Remote_cnt_st;

Irda_ROOT	unsigned char	RxRemoteBuff[4];
#define	rRmtKyCodeC	RxRemoteBuff[3]
#define	rRmtKyCode 		RxRemoteBuff[2]
#define	rRmtCustomH	RxRemoteBuff[1]
#define	rRmtCustomL	RxRemoteBuff[0]

/*char	rRmtCodeBuff;*/
Irda_ROOT	BYTE_FIELD	RmtFlag;	/*bit 3 ~ 0 use as option diode status*/
#define	bRmtCodeRdy		RmtFlag.b0
#define	bRmtRptKey		RmtFlag.b1
#define	bRmtKyRpting	RmtFlag.b2
#define	bRemoteData		RmtFlag.b3
#define	b1sWinkDpOn		RmtFlag.b4
#define	bNewKey			RmtFlag.b5
#define	bIRInt			RmtFlag.b6





/*
 *    infrared remote uPD6122 format pulse width define
 */
#define	_Data1Max    		2500//50*2//24	/* Remote code 1 Max width */
#define	_Data1Min    		2000//26*2//13  	/* Remote code 1 Min width */

#define	_Data0Max    		1500//_Data1Min /* Remote code 0 Max width */
#define	_Data0Min    		900//15*2//5  	/* Remote code 0 Min width */

#define	_LeadCodeMax 		14000//285*2//115  	/* Remote lead code  Max width */
#define	_LeadCodeMin 		12000//200*2//97  	/* Remote lead code  Min width */

#define	_RptCodeMax  		_LeadCodeMin*2	/* Remote lead code  Max width */
#define	_RptCodeMin  		11000//180*2//70/*79  	/* Remote lead code  Min width*/

#define  _TotalBits    			32

/*
 *  remote key code define
 */
#define	_CustomCode1		0x0

#define	_RmtAudInc			0x10	/* remote Vol + key */
#define	_RmtAudDec			0x11	/* remote Vol + key */
#define	_RmtSeekInc		0x12	/* remote Seek + key */
#define	_RmtSeekDec		0x13	/* remote Seek - key */
#define	_RmtTuneInc		0x14	/* remote Tune + key */
#define	_RmtTuneDec		0x15	/* remote Tune - key */
#define	_RmtPSNumInc		0x19	/* radio Preset station Up */
#define	_RmtPSNumDec		0x20	/* radio Preset station Down */
#define	_kcRmtBand			0x0e
/*
 * remote procedure status define
 */
#define	_RmtWtHeadCode		0
#define	_RmtWtFrameEnd		1
#define	_RmtLongKyCfm			2
#define	_RmtWtStartRpt			3
#define	_RmtRpting				4
#define	_RmtPrsing				5

/*functions forward statement*/
extern	void    RemoteProc(void );
extern	void	RstRmtDecode( void);
extern void	RemoteInt(void);
