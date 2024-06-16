
> 提示：文章写完后，目录可以自动生成，如何生成可参考右边的帮助文档

@[TOC](文章目录)

---

# 前言
这篇文章主要记录ESP32在arduino环境下如何接入阿里云的操作，对于阿里云物联网平台的配置和操作这里不做过多解释，主要参考这篇文章：链接: [【物联网】手把手完整实现STM32+ESP8266+MQTT+阿里云+APP应用——第1节-阿里云配置+MQTT.fx模拟与使用AT命令发布订阅消息
](https://blog.csdn.net/m0_61712829/article/details/135248254?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522171817599516800188537134%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=171817599516800188537134&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-135248254-null-null.142^v100^pc_search_result_base6&utm_term=%E9%98%BF%E9%87%8C%E4%BA%91&spm=1018.2226.3001.4187)
我这里只做些补充说明。当然也可以去看阿里云物联网平台的官方文档，也很简单。

# 一、MQTTX客服端连接
前言中的文章博主使用的MQTT客服端测试软件是MQTT.fx这个软件要1.7的才免费，最新的版本好像是要钱的，我后面就没继续用。在我以前做毕设的时候用过一个叫 [EMQX](https://www.emqx.com/zh/try?tab=cloud)平台的MQTT客服端软件，很好用且免费。
下载后无脑安装就行。
![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/df0bc63fff7643ff834e3c2386d95e39.png)

软件打开后就是这个界面，我们需要填的主要上面这四个参数***clientId、username、passwd、Host***
这几个参数，打开你们的阿里云物联网平台找到设备然后点开MQTT连接参数就可以看到了
![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/82ebff3bb4eb464aabc65ac4b81e7b1f.png)
注意**Host**这个参数对应的是**mqttHostUrl**这个参数。
填完参数后要几点修改MQTT Version 要改成3.1。阿里云的MQTT Version5连接方式是不一样的。这里一定要注意修改，而且我用的ESP32的***PubSubClient***库也只支持3.1和3.1.1版本。
配置完成点击连接就可以了，然后到阿里云的物联网平台设备界面刷新一下看看是否连接上了![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/2b3b746454d74a0c9465043202c4a07d.png)
如果没有连接上，可以打开日志参看一下，是什么原因。不过大部分原因都是参数写错了。
# 二、MQTTX消息订阅和发布
## 2.1发布
发布格式，这里主要讲这么发布物模型通信**Topic**。
打开阿里云的产品文档找到物联网平台在快速入门就有很详细的介绍
![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/a6119cd5c6c243078afb27e9e343f8f9.png)
```bash
/sys/a1****/device1/thing/event/property/set
```
要修改两个参数：a1***对应你设备的**ProductKey**
							device1对应你设备的**DeviceName**
打开MQTTX，在对应发布框中把物模型Topic信息填进去
![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/434f1b515f2c4e4489e4aece9d2f9778.png)
发送Topic的具体格式也可以在产品文档中找到。

![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/dd249b9107a94323ac5427e404c1ca6c.png)
在设备接入——使用协议自主接入——基于Alink协议自主开发——设备属性、事件、服务
具体的数据自己找吧。
```bash
{
  "id": "123",
  "version": "1.0",
  "sys":{
      "ack":0
  },
  "params": {
         "lcon": 100
    },
  "method": "thing.event.property.post"
}
```
一般要修改的是"**params**"里的标识符。这个在你创建产品和设备的时候，自己添加的模块属性，比如我这里的是"**lcon**"
![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/2fc2d5a774d64340abf4366794fcfa4f.png)
不知怎么创建的，把前言的那篇文章好好看几遍。

### 2.1.1发布测试
![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/317e112b650145c58367189009c217ba.png)
在这个对话框里填入刚刚的json格式字符串，点击发送。
在阿里云物联网平台查看一下数据是否接受到了
![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/65a8b3a537c0483aaa34b70ff9dd5552.png)

## 2.2 订阅

订阅格式，这里主要讲这么订阅物模型通信**Topic**。
打开阿里云的产品文档找到物联网平台在快速入门就有很详细的介绍
![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/5cba9748b4244d7c80b83840ecc0fc26.png)

```bash
/sys/a1****/device1/thing/event/property/post
```
要修改两个参数：a1***对应你设备的**ProductKey**
							device1对应你设备的**DeviceName**
![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/7062fb56a09a40ed8e6330925ba1b25f.png)

![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/b21ea6347a0e437db6b2386989e23f34.png)
点击这个位置把刚刚修改好的Topic添加进去就可以了。
### 2.2.1订阅测试
配置完成后，回到阿里云打开在线调试页面，选择你之前设置好的属性，不知道属性是什么的去看前言中的文章。
![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/2656f814e6984003b4dbd4beeb282064.png)
选择好你的属性点击获取，就可以到你之前订阅的Topic的内容了。
这里订阅设置完成后也可以点击设置，下发数据到客服端。
![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/285a19bf36d44a66a54534f891ae29b9.png)
# 三、ESP32接入阿里云
参考文章[ESP8266接入阿里云（Arduino）](https://blog.csdn.net/xyx0610/article/details/123934145?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522171817961816800222860552%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=171817961816800222860552&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~hot_rank-5-123934145-null-null.142^v100^pc_search_result_base6&utm_term=arduino%20%E9%98%BF%E9%87%8C%E4%BA%91%20%20&spm=1018.2226.3001.4187)
首先我使用的是VScode中的Plaformio搭建的arduino环境，这个环境怎么搭建网上教程很多，这里不赘述了。
在开始写代码测试时要先安装以下几个库：**PubSubClient**和**ArduinoJson**
**ArduinoJson**建议使用6.21.4版本。

```cpp
在这里插入代码片#include "Arduino.h"
#include "WiFi.h"
#include "PubSubClient.h"
#include <ArduinoJson.h> // 引入ArduinoJson库

/* 连接您的WIFI SSID和密码 */
#define WIFI_SSID         "你的wifi名称"
#define WIFI_PASSWD       "你的wifi密码"

/* 线上环境域名和端口号 */
#define MQTT_SERVER       "*******************************"
#define MQTT_PORT         1883 //端口号
#define MQTT_USERNAME     "*******************************"
#define CLIENT_ID         "*******************************"
#define MQTT_PASSWORD     "*******************************"

//发布消息用的topic
#define PubTopic "/sys/****/*****/thing/event/property/post"
#define SubTopic "/sys/****/*****/thing/service/property/set"
WiFiClient espClient;
PubSubClient client(espClient);

```
除了WIFI名称和密码根据你自己的wifi来修改，其他的参数都是上文都有提到。

```cpp
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
```
这个代码是将ESP32的数据发布出去的函数。
需要修改的就  `doc["params"]["lcon"] = 120;`这句里面的标识符，和对应的数据。

```cpp
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
```
这是订阅信息的，接受来着阿里云下发的数据或其他设备发布的数据。

```cpp
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
```
这是一个回调函数，ESP32收到消息的时候就会调用这个函数，有点像中断。

```cpp
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
```
WiFi的初始化和连接MQTT服务器，没什么好说的，自己看吧。

```cpp
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

```
这个是MQTT连接函数。

```cpp
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

```
# 总结
虽然说本文是讲如何用ESP32连接阿里云，但主要的内容是如何用客服端连接阿里云的服务器。
因为我是默认你们会ESP32的，不会使用esp32的基本功能来看这篇文章或者其他文章，我个人觉得是没必要的。但如果你会使用ESP32使用MQTT协议链接阿里云的服务器，其实就是一个客服端接入的过程。只要明白前面MQTTX的连接，其他的问题就很简单了。
参考文章：

 [【物联网】手把手完整实现STM32+ESP8266+MQTT+阿里云+APP应用——第1节-阿里云配置+MQTT.fx模拟与使用AT命令发布订阅消息
](https://blog.csdn.net/m0_61712829/article/details/135248254?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522171817599516800188537134%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=171817599516800188537134&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-135248254-null-null.142^v100^pc_search_result_base6&utm_term=%E9%98%BF%E9%87%8C%E4%BA%91&spm=1018.2226.3001.4187)
[ESP8266接入阿里云（Arduino）](https://blog.csdn.net/xyx0610/article/details/123934145?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522171817961816800222860552%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=171817961816800222860552&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~hot_rank-5-123934145-null-null.142^v100^pc_search_result_base6&utm_term=arduino%20%E9%98%BF%E9%87%8C%E4%BA%91%20%20&spm=1018.2226.3001.4187)
