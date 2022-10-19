
#include	"GUI.h"



/*=====================================================================*/

static	void 	ADS7843_SPI_Start(void);
static	void 	ADS7843_WrCmd(U8 cmd);
static	 U16 	ADS7843_Read(void);

/*=====================================================================*/

/*=====================================================================*/
// A/D 通道选择命令字和工作寄存器
#define	CHX 	0x90 	//通道Y+的选择控制字	//0x94
#define	CHY 	0xd0	//通道X+的选择控制字	//0xD4

//写ADS7843的IO控制口

#define	SPI_MISO		((rPDATC&(1<<0)))
#define	SPI_MOSI(a)		rPDATC = ( rPDATC&(~(1<<2)) ) | ((a&1)<<2)
#define	SPI_SCK(a)		rPDATC = ( rPDATC&(~(1<<4)) ) | ((a&1)<<4)
#define	SPI_CS(a)		rPDATC = ( rPDATC&(~(1<<3)) ) | ((a&1)<<3)

//读ADS7843的IO控制口
	
#define TP_IRQ		((rPDATG>>3) & 0x1)

/*=====================================================================*/


/*=====================================================================*/
/*
void __irq ADS7843_Interrupt_Serve( void )
{
	
	//Touch_Screen_Press_100ms();
	
	//TP_Time_Count=0;
	
	
	rI_ISPC = BIT_EINT2 ;	//clear pending_bit

}
*/
/*=====================================================================*/


static	int	ADS7843_Init(void)

{
//SPI_CS	->	GPC3			Output
//SPI_SCK	->	GPC4			Output
//SPI_MOSI	->	GPC2			Output
//SPI_MISO	->	GPC0			Input
//IRQ		->	GPG3(Ex_Int3)	Input

	
	U16 j;

	////
	
	rPCONC &= (~((3<<8)|(3<<6)|(3<<4)|(3<<0)));
	rPCONC |= ((1<<8)|(1<<6)|(1<<4)|(0<<0));
	rPUPC  &= (~((1<<4)|(1<<3)|(1<<2)|(1<<0)));
	
	rPCONG &= (~(3<<6));
	rPUPG  &= (~(1<<3));
    
    
    ADS7843_SPI_Start();
    ADS7843_WrCmd( CHX ) ;
	for( j = 0; j < 100; j++ );		//200ns
	ADS7843_Read() ;
	
	ADS7843_WrCmd( CHY ) ;
	for( j = 0; j < 100; j++ );		//200ns
	ADS7843_Read() ;

	return 1;
	
	
}

/*=====================================================================*/

static	int	ADS7843_IsDown(void)		
{
		
		
		if(!TP_IRQ)
		{
			return 1;
		}
		else
		{
			return 0;
		}
		

}

/*=====================================================================*/

static	void ADS7843_SPI_Start( void )
{
    SPI_SCK(0);
	SPI_CS(1);
	SPI_MOSI(1);
    SPI_SCK(1);
    SPI_CS(0);	      		//芯片允许
   
}

/*=====================================================================*/

static	void ADS7843_WrCmd(U8 cmd)
{
	U8 buf, i, j ;
	
	
    SPI_SCK(0);
    
	for( i = 0; i < 8; i++ )
	{
		buf = (cmd >> (7-i)) & 0x1 ;	//MSB在前,LSB在后
		SPI_MOSI(buf);	 	//时钟上升沿锁存DIN
		
		for( j = 0; j < 50; j++ );	//200ns
		SPI_SCK(0);			//开始发送命令字
		
		for( j = 0; j < 50; j++ );	//200ns
		SPI_SCK(1);			//时钟脉冲，一共8个
		
		for( j = 0; j < 50; j++ );	//200ns
		SPI_SCK(0);			//时钟脉冲，一共8个
		
	}
	
	
}

/*=====================================================================*/

static U16 ADS7843_Read(void)
{
	U16 buf ;
	
	U8 i, j ;
	
	
	
	buf=0;
	
	for( i = 0; i < 12; i++ )
	{
		buf = buf << 1 ;
		
		SPI_SCK(1);
		for( j = 0; j < 50; j++ );		//200ns
		SPI_SCK(0);			
		for( j = 0; j < 50; j++ );	//时钟下降沿读取,一共12个,MSB在前,LSB在后

		if ( SPI_MISO )	
		{
			buf = buf + 1 ;
		}
	
		for( j = 0; j < 50; j++ );		//200ns
	
		
		
	}
	
	
		
	
	return( buf ) ;
}

/*=====================================================================*/


static	int	ADS7843_Rd_Addata(int *X_Addata,int *Y_Addata)
{

	U16		i,j,x_addata,y_addata;

	ADS7843_SPI_Start() ;
	for( j = 0; j < 10; j++ );			//40ns
//	while ( TP_BUSY ) ;					//如果BUSY，等待直到转换完毕，这个可以不用
	
	x_addata=0;
	y_addata=0;
	
	
	for(i=8;i>0;i--)					//采样8次.
	{
		ADS7843_WrCmd( CHX ) ;
		for( j = 0; j < 50; j++ );		//200ns
		y_addata += ADS7843_Read() ;
		for( j = 0; j < 50; j++ );		//200ns

		ADS7843_WrCmd( CHY ) ;
		for( j = 0; j < 50; j++ );		//200ns
		x_addata += ADS7843_Read() ;
		for( j = 0; j < 50; j++ );		//200ns
   
	}
	
	////
	
	
	x_addata>>=3;
	y_addata>>=3;						//AVG.
	////
	
	*X_Addata=x_addata;
	*Y_Addata=y_addata;
	return 1;

}

/*=====================================================================*/


static	int	GetTSCalData(U8 *buf)
{
	U8	i,size;
	U8	dat[20];
	////
		
	size=sizeof(struct TS_CAL_DAT );

	////
	for(i=0;i<size;i++){
	
		IIC_Read(System_EEPROM,(TS_CAL_DAT_ADR_BASE+i),&buf[i]);
			
	}
	
	////
	#if(GUI_DBG)
//	Uart_Output_Hex(0,0,20,buf);
	#endif
}

/*=====================================================================*/

static	int	SaveTSCalData(U8 *buf)
{

	U8	i,size;
	int j;
	////
	Uart_Puts(0,"SaveTSCalData\n");
	size=sizeof(struct TS_CAL_DAT );

	////
	for(i=0;i<size;i++){
	
		//Uart_Printf(0,"-> %d\n",i);
		
		IIC_Write(System_EEPROM,(TS_CAL_DAT_ADR_BASE+i),*(buf+i));
		//Delay_n15ms(30/15);
		for(j=100000;j>0;j--);
	}
	
	////

	
}

/*=====================================================================*/

static	int ADS7843_Subpend(void)
{
	#if(GUI_DBG)
	Uart_Puts(0,"ADS7843 Subpend!\n");
	#endif
}

/*=====================================================================*/

static	int ADS7843_Resume(void)
{

	#if(GUI_DBG)
	Uart_Puts(0,"ADS7843 Resume!\n");
	#endif
}

/*=====================================================================*/

struct	GUI_IAL_TS	Fit239A_TS_ADS7843={

		ADS7843_Init,
		GetTSCalData,
		SaveTSCalData,
		ADS7843_IsDown,
		ADS7843_Rd_Addata,
		ADS7843_Subpend,
		ADS7843_Resume,
		

};


/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/

