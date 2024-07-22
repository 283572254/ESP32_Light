#include "aliyun.h"

WiFiClient espClient;
PubSubClient client(espClient);

/**
 * @brief 发布消息
 *
 * 创建一个StaticJsonDocument对象，设置JSON数据，并将其序列化为字符串，
 * 然后使用MQTT客户端发布该JSON字符串到指定的主题。
 *
 * @note 请确保在调用此函数之前，MQTT客户端已经正确连接。
 */
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


/**
 * @brief 处理 JSON 消息
 *
 * 将传入的 JSON 字符串解析并处理其中的数据。
 *
 * @param message JSON 消息字符串
 */
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
  int LightLcon = doc["params"]["LightLcon"]; // 1-10000
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

// void prams_set(int data)
// {

// }


/**
 * @brief 回调函数
 *
 * 当接收到消息时，此回调函数将被调用。
 *
 * @param topic 主题字符串指针
 * @param payload 消息负载字节指针
 * @param length 消息负载的长度（以字节为单位）
 */
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



/**
 * @brief 检查 MQTT 连接
 *
 * 循环检查 MQTT 客户端是否已连接。如果未连接，则尝试连接 MQTT 服务器，并输出连接状态信息。
 *
 * 如果连接成功，则在串口输出 "MQTT Connected"。
 * 如果连接失败，则在串口输出 "MQTT Connect err: " 加上 MQTT 客户端的当前状态。
 * 连接失败后，程序会等待 5 秒后再次尝试连接。
 */
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