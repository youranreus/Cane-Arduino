/**
*-----------------------------------------
* @Title: cane
* @Package cane
* @Description: main code for cane
* @author ZZF
* @date 2018.11.4
* @version V1.0
*-----------------------------------------
*/

//Include

#include "Accelerometer_MMA7660-master/MMA7660.h"
//#include "NEMAGPS.h"
#include "Adafruit_NeoPixel/Adafruit_NeoPixel.h"
#include <SoftwareSerial.h>
#include <Wire.h>
//#include "pt-1.4/pt.h"
#define PIN 7


//Declare Serial

SoftwareSerial gpsSerial(2,3);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(20, PIN, NEO_GRB + NEO_KHZ800);
MMA7660 accelemeter;

//================================
//Declare value
//================================
const int pinPlay = 5;
const int switchPin = 4;
int switchState = 0;
//const int ButtonPin = 4;
//int ButtonState = 0;
int lont, lati;
float x, y, z;
char cmd;
int PhyHelpStatus = 0;
int SStatus = 0;
int gpsStatus = 0;
String cmds;

uint8_t rb[7][3] = { {255, 0, 0}, {255, 125, 0}, {255, 255, 0},{0, 255, 0}, {0, 155, 255}, {0, 0, 255}, {139, 0, 255}};

void setup()
{
	
	//gpsSerial.begin(9600);
	Serial.begin(9600);
	accelemeter.init();
	

	//清除串口缓存
	while (Serial.read() >= 0) {}

	//发音元件设置
	pinMode(pinPlay, OUTPUT);
	digitalWrite(pinPlay, HIGH);
	//pinMode(ButtonPin, INPUT);

	//求救灯设置
	strip.begin();
	
}
void loop() 
{

	while (Serial.available() > 0) {
		cmd = Serial.read();
	}
	
		
	//获取按钮状态
	switchState = digitalRead(switchPin);

	//获取加速度状态
	accelemeter.getAcceleration(&x, &y, &z);


	//加速度判定
	if_boom(x, y, z);

	if (switchState == HIGH)
	{
		if (SStatus == 0) 
		{
			Serial.print("s");
			SStatus = 1;
		}
		else 
		{
			PhyHelpStatus = 0;
		}
		PhyHelpStatus = 1;
	}
	else 
	{
		PhyHelpStatus = 0;
	}
	
	SendHelp_phy();

	switch (cmd)
	{
		case 'a':

			PhyHelpStatus = 1;
			for (int x = 0; x < 5; x++) 
			{
				SendHelp_phy();
			}
			PhyHelpStatus = 0;

			break;		

		default: 
			break;
	}

	//清除cmd缓存
	cmd =' ';
}


void SendHelp_phy()
{
	if (PhyHelpStatus == 1) {
		
		digitalWrite(pinPlay, LOW);
		delay(100);
		digitalWrite(pinPlay, HIGH);
		delay(1000);
		int count = 0;
		for (count = 0; count < 30; count = count + 1) {
			strip.setPixelColor(count, 200, 0, 0);
			strip.setBrightness(100);
			strip.show();
		}
		delay(1000);
		strip.setBrightness(0);
		strip.show();

	}
	
}
void if_boom(float ax, float ay, float az)
{
	int i = 0;
	if (ax >= 1 && ay >= 0.8)
	{

		//发送跌倒警报
		PhyHelpStatus = 1;
		for (int x = 0; x < 5; x++)
		{
			SendHelp_phy();
		}
		PhyHelpStatus = 0;

	}
	if (ax >= 1 && az >= 1.3)
	{

		//发送跌倒警报
		PhyHelpStatus = 1;
		for (int x = 0; x < 5; x++)
		{
			SendHelp_phy();
		}
		PhyHelpStatus = 0;

	}
	if (ay >= 0.8 && az >= 1.3)
	{

		//发送跌倒警报
		PhyHelpStatus = 1;
		for (int x = 0; x < 5; x++)
		{
			SendHelp_phy();
		}
		PhyHelpStatus = 0;

	}

	if (ax <= -1 && ay <= -0.8)
	{

		//发送跌倒警报
		PhyHelpStatus = 1;
		for (int x = 0; x < 5; x++)
		{
			SendHelp_phy();
		}
		PhyHelpStatus = 0;

	}
	if (ax <= -1 && az <= -1.3)
	{

		//发送跌倒警报
		PhyHelpStatus = 1;
		for (int x = 0; x < 5; x++)
		{
			SendHelp_phy();
		}
		PhyHelpStatus = 0;

	}
	if (ay <= -0.8 && az <= -1.3)
	{

		//发送跌倒警报
		PhyHelpStatus = 1;
		for (int x = 0; x < 5; x++)
		{
			SendHelp_phy();
		}
		PhyHelpStatus = 0;

	}
}