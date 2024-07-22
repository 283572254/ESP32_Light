#ifndef ALIYUN_H
#define ALIYUN_H
#include "PubSubClient.h"
#include <ArduinoJson.h> // 引入ArduinoJson库
#include "WiFi.h"
#include "PubSubClient.h"
#include <ArduinoJson.h> // 引入ArduinoJson库


/* 连接您的WIFI SSID和密码 */
#define WIFI_SSID         "Redmi K60"
#define WIFI_PASSWD       "201910088047"
/* 线上环境域名和端口号 */
#define MQTT_SERVER       "iot-06z00faz7aul5a6.mqtt.iothub.aliyuncs.com"
#define MQTT_PORT         1883 //端口号
#define MQTT_USERNAME     "MingTong&hyldTvgtqaQ"
#define CLIENT_ID         "hyldTvgtqaQ.MingTong|securemode=2,signmethod=hmacsha256,timestamp=1718180470466|"
#define MQTT_PASSWORD     "04842f2fdf439644cf6ade713edb14b62ede8d4a74ca895ab8058686d4987536"

//发布消息用的topic
#define PubTopic "/sys/hyldTvgtqaQ/MingTong/thing/event/property/post"
#define SubTopic "/sys/hyldTvgtqaQ/MingTong/thing/service/property/set"

extern PubSubClient client;

void pubMsg();
void processJsonMessage(char* message);
void wifiInit();
void mqttCheckConnect();
#endif // !ALIYUN_H
