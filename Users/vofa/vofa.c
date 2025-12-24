#include "vofa.h"
#include "rs485.h"


uint8_t c_data[4];

unsigned char DataScope_OutPut_Buffer[44] = {0x00,0x00,0x80,0x7f,
											0x00,0x00,0x80,0x7f,
											0x00,0x00,0x80,0x7f,
											0x00,0x00,0x80,0x7f,
											0x00,0x00,0x80,0x7f,
											0x00,0x00,0x80,0x7f,
											0x00,0x00,0x80,0x7f,
											0x00,0x00,0x80,0x7f,
											0x00,0x00,0x80,0x7f,
											0x00,0x00,0x80,0x7f,
											0x00,0x00,0x80,0x7f};


//函数说明：将单精度浮点数据转成4字节数据并存入指定地址
//附加说明：用户无需直接操作此函数
//target:目标单精度数据
//buf:待写入数组
//beg:指定从数组第几个元素开始写入
//函数无返回
void Float2Byte(float *target,unsigned char *buf,unsigned char beg)
{
	unsigned char *point;
	point = (unsigned char*)target;	  //得到float的地址
	buf[beg]   = point[0];
	buf[beg+1] = point[1];
	buf[beg+2] = point[2];
	buf[beg+3] = point[3];
}


//函数说明：将待发送通道的单精度浮点数据写入发送缓冲区
//Data：通道数据
//Channel：选择通道（1-10）
//函数无返回
void Formate_Channel_Data(float Data,unsigned char Channel)
{
	if (Channel > 9){
		return;  //通道个数大于10或等于0，直接跳出，不执行函数
	}
	else{
		switch (Channel){
		case 0:  Float2Byte(&Data,DataScope_OutPut_Buffer,0); break;
		case 1:  Float2Byte(&Data,DataScope_OutPut_Buffer,4); break;
		case 2:  Float2Byte(&Data,DataScope_OutPut_Buffer,8); break;
		case 3:  Float2Byte(&Data,DataScope_OutPut_Buffer,12); break;
		case 4:  Float2Byte(&Data,DataScope_OutPut_Buffer,16); break;
		case 5:  Float2Byte(&Data,DataScope_OutPut_Buffer,20); break;
		case 6:  Float2Byte(&Data,DataScope_OutPut_Buffer,24); break;
		case 7:  Float2Byte(&Data,DataScope_OutPut_Buffer,28); break;
		case 8:  Float2Byte(&Data,DataScope_OutPut_Buffer,32); break;
		case 9:  Float2Byte(&Data,DataScope_OutPut_Buffer,36); break;
		}
	}
}


/**
 * @arr			浮点数组
 * @ChannelNum	通道数
 */
void JustFloat_Send(float *arr,uint8_t ChannelNum)
{
	for (uint8_t i=0;i<ChannelNum;i++){
		Formate_Channel_Data(*(arr+i),i);
	}

	RS485_Send(DataScope_OutPut_Buffer, 4*ChannelNum+4);
}


/*
 * 根据串口信息进行PID调参
 */
void VOFA_USART_PI_Adjust(uint8_t *DataBuff ,uint8_t on)
{
	;
}

