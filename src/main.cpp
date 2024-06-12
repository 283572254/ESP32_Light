#include "Arduino.h"
#include "HardwareSerial.h"        //调用串口库
#define UART_FULL_THRESH_DEFAULT 2048         //修改缓冲区大小，这个是HardwareSerial.h文件中说的修改方法，我试了，并没有发挥作用
#define CJ_RxPin 34                //设置RX管脚
#define CJ_TxPin 35                //设置TX管脚
HardwareSerial Serial_CJ(0);       //定向串口1

void Collect_Callback(){               
  String Collect_Data = "";                     //定义一个String类型的变量
  while(Serial_CJ.available()){                 //用While判断缓冲区是否有内容
    Collect_Data += char(Serial_CJ.read());     //取出缓冲区内容
  } 
  Serial.print(Collect_Data);                     //输出取出的内容
  Collect_Data = "";                              //清空内容
 }
void setup() {
	Serial.begin(115200);           //设置初始化串口0
	Serial_CJ.begin(115200,SERIAL_8N1,CJ_RxPin,CJ_TxPin);  //初始化串口1，初始化参数可以去HardwareSerial.h文件中查看
	Serial_CJ.onReceive(Collect_Callback);    //定义串口中断函数
}
void loop(){
	delay(1000);             //延时
}


