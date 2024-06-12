#include "Arduino.h"
#include "WiFi.h"
#include "PubSubClient.h"

/* 连接您的WIFI SSID和密码 */
#define WIFI_SSID         "Redmi K60"
#define WIFI_PASSWD       "201910088047"
 
/* 线上环境域名和端口号 */
#define MQTT_SERVER       "你的域名"
#define MQTT_PORT         1883//端口号
#define MQTT_USRNAME      "你的username"
#define CLIENT_ID         "你的clientId"
#define MQTT_PASSWD       "你的password"
 
//发布消息用的topic
#define PubTopic "你的topic"


//wifi初始化
void wifiInit()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("WiFi not Connect");
  }
 
  Serial.println("Connected to AP");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
  Serial.print("espClient [");
 

}

void setup()
{
/* initialize serial for debugging */
    Serial.begin(115200); //开启串口
    Serial.println("Demo Start");
    wifiInit();//连接wifi

}
 
// the loop function runs over and over again forever
void loop()
{
  delay(1000);
}

