#include "Arduino.h"

#include "aliyun.h"




void setup() {
  Serial.begin(115200); //开启串口
  Serial.println("Demo Start");
  wifiInit(); //连接wifi
  mqttCheckConnect(); //连接MQTT服务器
  pubMsg();
}

void loop() {
  client.loop(); //保持客户端的连接
}

