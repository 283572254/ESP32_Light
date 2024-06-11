#include "Arduino.h"
#include "BluetoothSerial.h"

// 检查是否有BluetoothSerial库
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_BT_Device"); // 设置蓝牙设备名称
  Serial.println("Bluetooth device is ready to pair");
}

void loop() {
  if (SerialBT.available()) {
    // 创建一个字符串来保存接收到的16进制数据
    String hexData = "";

    // 当有数据可读时，继续读取
    while (SerialBT.available()) {
      int byteRead = SerialBT.read(); // 读取一个字节

      // 将读取到的字节转换为16进制字符串
      char hexStr[3]; // 两个字符加上null终止符
      sprintf(hexStr, "%02X", byteRead); // 格式化为两位16进制数

      // 将转换后的16进制字符串添加到hexData字符串中
      hexData += hexStr;
      hexData += " "; // 为了可读性，在每个字节之间添加一个空格
    }

    // 打印接收到的16进制数据
    Serial.print("Received data in hex: ");
    Serial.println(hexData);
  }
  delay(100); // 稍微延迟一下，以免过于频繁地读取
}