// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
	Name:       cane_v2.ino
	Created:  2018/11/12 18:14:52
	Author:     DESKTOP-GHQONMD\季悠然
*/

// Define User Types below here or use a .h file
//
#define BLINKER_PRINT Serial
#define BLINKER_MQTT
#include <Blinker.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
//32b222024502
//685bb612339c
char auth[] = "32b222024502";
char ssid[] = "YOURANREUS";
char pswd[] = "111222333";
char cmd;
float result_LONG;
float result_LAT;
//int cmdsc = 0;
//char cmds[200];
const char* host = "sos.net"; //需要访问的域名
const int httpsPort = 80;  // 需要访问的端口
String url = "";   // 需要访问的地址
String Igps = "gps";

//注册Blinker组件
BlinkerButton Button_help("btn-hlp");
BlinkerButton Button_gps("btn-gps");

void setup()
{
	//启动串口
	Serial.begin(9600);
	while (Serial.read() >= 0) {}

	//Blinker设置
	#if defined(BLINKER_PRINT)
	  //BLINKER_DEBUG.stream(BLINKER_PRINT);
	  //BLINKER_DEBUG.debugAll();
	#endif

	// 初始化Blinker，连接WIFI
	Blinker.begin(auth, ssid, pswd);
	Blinker.delay(500);

	// 注册回调函数
	//Blinker.attachData(dataRead);
	Button_help.attach(button_help_callback);
	Button_gps.attach(button_gps_callback);
	//Blinker.attachHeartbeat(heartbeat);

	//向家人发送启动信息
	Blinker.push("家人出门了");

}

void loop() {

	//检测Blinker运行情况
	Blinker.run();
	//获取从2号板获得的指令
	while (Serial.available() > 0) {
		cmd = Serial.read();
	}

	


	//处理指令
	switch (cmd)
	{
		case 's':
		{
			Blinker.delay(1500);
			delay(1500);

			//向家人发送警报
			Blinker.push("家人跌倒了");//APP推送
			Blinker.sms("家人跌倒了");//短信推送
			//获取GPS
			result_LAT = Blinker.gps(LAT);
			result_LONG = Blinker.gps(LONG);
			//进行GPS数据格式转换
			char sLat[50];
			dtostrf(result_LAT, 9, 0, sLat);
			String ssLat = sLat;
			
			char sLong[50];
			dtostrf(result_LONG, 9, 0, sLong);
			String ssLong = sLong;
			//合成http请求路径
			url = "/Api/update?email=1208944952@qq.com&long="+ ssLong + "&lati="+ ssLat +"&type=1";

			//Serial.println(url);
			//向HTTP发送请求
			http_get(url);

			//清空url以便下次使用
			url = "";

			break; 
		}
		default:
			
			break;
	}

	//清空cmd以便下次使用
	cmd = ' ';
}



//=======================================================
// HTTP
//=======================================================
void http_get(String hurl)
{
	WiFiClient client;

	/**
	 * 测试是否正常连接
	 */
	if (!client.connect(host, httpsPort)) {
		Serial.println("connection failed");
		return;
	}
	delay(10);

	String postRequest = (String)("GET ") + url + "/ HTTP/1.1\r\n" +
		"Content-Type: text/html;charset=utf-8\r\n" +
		"Host: " + host + "\r\n" +
		"User-Agent: BuildFailureDetectorESP8266\r\n" +
		"Connection: Keep Alive\r\n\r\n";
	//Serial.println(postRequest);
	client.print(postRequest);
	String line = client.readStringUntil('\n');
	while (line.length() != 0) {
		//Serial.println(line);
		line = client.readStringUntil('\n');
	}
	//Serial.println(line);
	client.stop();
	delay(3000);
}



//=======================================================
// Blinker回调函数
//=======================================================

//输出从APP端接受的数据
void dataRead(const String & data)
{
	BLINKER_LOG("Blinker readString: ", data);

	uint32_t BlinkerTime = millis();
	Blinker.print(BlinkerTime);
	Blinker.print("millis", BlinkerTime);
}
//报警按钮回调
void button_help_callback(const String & state) {
	if (state == "sos") 
	{
		Serial.println("a");
	}
}
void button_gps_callback(const String & state) {
	if (state == "gg")
	{
		Blinker.delay(1000);
		delay(1000);

		result_LAT = Blinker.gps(LAT);
		result_LONG = Blinker.gps(LONG);
		char sLat[50];
		dtostrf(result_LAT, 8, 0, sLat);
		String ssLat = sLat;

		//Serial.println(ssLat);

		char sLong[50];
		dtostrf(result_LONG, 9, 0, sLong);
		String ssLong = sLong;

		url = "/Api/update?email=1208944952@qq.com&long=" + ssLong + "&lati=" + ssLat + "&type=2";

		//Serial.println(url);

		http_get(url);
		url = "";
	}
}
// 心跳包回调
//void heartbeat()
//{
//	
//}
