#include "Arduino.h"
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

// 定义一个short型数组来存储接收到的数据
// 数组大小为512，因为每个short可以存储两个字节，所以可以存储1024个字节
short receivedData[512];
int dataIndex = 0; // 用于跟踪数组中当前存储位置的索引

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); // 蓝牙设备名称
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  static byte byteBuffer[2]; // 用于暂存两个字节
  static int bufferIndex = 0; // byteBuffer的索引

  // 检查是否有从蓝牙接收到的数据
  while (SerialBT.available() > 0 && dataIndex < 512) {
    int byteRead = SerialBT.read(); // 读取一个字节

    // 检查read()是否真的读取到了数据
    if (byteRead != -1) {
      byteBuffer[bufferIndex++] = (byte)byteRead; // 存储到buffer中

      // 检查是否收集了两个字节
      if (bufferIndex == 2) {
        // 将两个字节合并为一个short型变量
        // 注意：这里可能需要根据实际的字节序来调整
        receivedData[dataIndex++] = (short)((byteBuffer[0] << 8) | byteBuffer[1]);

        // 重置bufferIndex以便于接收下一对字节
        bufferIndex = 0;
      }
    }
  }
  
  // 在这里可以处理接收到的short型数组数据
}