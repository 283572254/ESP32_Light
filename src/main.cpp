#include "Arduino.h"
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
WiFiClient espClient;
PubSubClient client(espClient);


//发布一个字符串的消息
void pubMsg() {
  // 创建一个足够大的StaticJsonDocument对象来存储预期的JSON数据
  StaticJsonDocument<256> doc;

  // 设置JSON数据
  doc["id"] = "123";
  doc["version"] = "1.0";
  doc["sys"]["ack"] = 0;
  doc["params"]["lcon"] = 120;
  doc["method"] = "thing.event.property.post";

  // 序列化JSON数据为字符串
  char jsonOutput[256];
  serializeJson(doc, jsonOutput);

  // 使用修改后的pubMsg函数发送JSON字符串
  if (client.publish(PubTopic, jsonOutput)) {
    Serial.println("Publish success");
    // 连接成功后订阅主题
    client.subscribe(SubTopic);
  } else {
    Serial.println("Publish fail");
  }
}


void processJsonMessage(char* message) {
  StaticJsonDocument<256> doc; // 调整大小以适应您的需要
  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  // 从JSON中提取信息
  const char* method = doc["method"]; // "thing.service.property.set"
  const char* id = doc["id"]; // "248166957"
  int PowerSwitch_1 = doc["params"]["PowerSwitch_1"]; // 1 or 0
  const char* version = doc["version"]; // "1.0.0"

  Serial.print("Method: ");
  Serial.println(method);
  Serial.print("ID: ");
  Serial.println(id);
  Serial.print("PowerSwitch_1: ");
  Serial.println(PowerSwitch_1);
  Serial.print("Version: ");
  Serial.println(version);

  // 根据PowerSwitch_1的值执行操作
  if (PowerSwitch_1 == 1) {
    // 如果PowerSwitch_1为1，执行相应操作
    //digitalWrite(LED_PIN, HIGH); // 示例：打开LED
    Serial.println("LED turned ON");
  } else {
    // 如果PowerSwitch_1为0，执行相应操作
    //digitalWrite(LED_PIN, LOW); // 示例：关闭LED
    Serial.println("LED turned OFF");
  }
}


//收到消息后的回调函数
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  // 将payload转换为字符串
  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = '\0';

  // 打印原始消息
  Serial.println(message);

  // 调用processJsonMessage函数处理消息
  processJsonMessage(message);
}
//wifi初始化
void wifiInit() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(MQTT_SERVER, MQTT_PORT);   /* 连接WiFi之后，连接MQTT服务器 */
  client.setCallback(callback);
}


//连接到服务器
void mqttCheckConnect() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT Server...");
    if (client.connect(CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("MQTT Connected");
    } else {
      Serial.print("MQTT Connect err: ");
      Serial.println(client.state());
      delay(5000);
    }
  }
}


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

