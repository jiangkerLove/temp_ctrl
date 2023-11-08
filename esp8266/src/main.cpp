/*
  ESP8266-NodeMCU作为TcpClient连接到服务器
*/

#include <ESP8266WiFi.h>                        // 本程序使用ESP8266WiFi库

const char* ssid     = "jiangker-wifi";                // 需要连接到的WiFi名
const char* password = "jfp-wifi";             // 连接的WiFi密码
int gpio = 16;

void setup() {
  Serial.begin(115200);                         // 初始化串口通讯波特率为115200

  analogWriteFreq(25000);            // 频率设置为1kHz，即周期为1ms
  analogWriteRange(1000);           // 范围设置为1000，即占空比步长为1us
  analogWrite(gpio, 1000);              // GPIO2 - D4 - LED

  WiFi.mode(WIFI_STA);                          // 设置Wifi工作模式为STA,默认为AP+STA模式
  WiFi.begin(ssid, password);                   // 通过wifi名和密码连接到Wifi
  Serial.print("\r\nConnecting to ");           // 串口监视器输出网络连接信息
  Serial.print(ssid); Serial.println(" ...");   // 显示NodeMCU正在尝试WiFi连接

  int i = 0;                                    // 检查WiFi是否连接成功
  while (WiFi.status() != WL_CONNECTED)         // WiFi.status()函数的返回值是由NodeMCU的WiFi连接状态所决定的。
  {                                             // 如果WiFi连接成功则返回值为WL_CONNECTED
    delay(1000);                                // 此处通过While循环让NodeMCU每隔一秒钟检查一次WiFi.status()函数返回值
    Serial.print("waiting for ");
    Serial.print(i++); Serial.println("s...");
  }

  Serial.println("");                           // WiFi连接成功后
  Serial.println("WiFi connected!");            // NodeMCU将通过串口监视器输出"连接成功"信息。
  Serial.print("IP address: ");                 // 同时还将输出NodeMCU的IP地址。这一功能是通过调用
  Serial.println(WiFi.localIP());               // WiFi.localIP()函数来实现的。该函数的返回值即NodeMCU的IP地址。
}

const char* host = "192.168.10.235";
const uint16_t port = 9001;

void loop() {
  /* 新建一个WiFiClient类对象，作为TCP客户端对象 */
  WiFiClient tcpclient;

  /* 建立TCP连接 */
  Serial.print("connecting to "); Serial.print(host); Serial.print(':'); Serial.println(port);
  if (!tcpclient.connect(host, port)) {
    Serial.println("connection failed");        // 如果连接失败，则打印连接失败信息，并返回
    delay(3000);
    return;
  }

  while (true)
  {
    String str = tcpclient.readStringUntil('\n');
    if (str.length()==0)
    {
      analogWrite(gpio, 1000);
      break;
    }
    int value = str.toInt();
    analogWrite(gpio, value);
    Serial.println(str);
  }

}
