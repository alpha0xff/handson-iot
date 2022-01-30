#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Ambient.h>

const char* ssid = "XXXXXXXX";
const char* password = "XXXXXXXX";
const unsigned int ambientChannelId = 99999u; // AmbientチャネルId
const char* ambientWriteKey = "XXXXXXXX"; // Ambientライトキー

// Ambient
WiFiClient ambientClient;
Ambient ambient;

// 温度センサー：DS18B20
OneWire oneWire(15);
DallasTemperature sensors(&oneWire);

void setup() {
  // シリアル初期化
  Serial.begin(115200);

  // 温度センサー：DS18B20開始
  sensors.begin();
  
  // WiFi接続
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // WiFi接続完了待ち
  int wifi_wait_cnt=0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    wifi_wait_cnt++;
    if (wifi_wait_cnt > 120) {
      // 120秒待って接続できない場合は、再起動
      Serial.println("WiFi Connect Fail -> reboot");
      ESP.restart();
    }
  }

  // Ambientの初期化：チャネルIDとライトキーを指定
  ambient.begin(ambientChannelId, ambientWriteKey, &ambientClient);
}

void loop() {
  // 温度計測
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  String temp_s = String(temp);
  Serial.printf("温度=%s\n", temp_s);

  // Ambientにデータ送信
  ambient.set(1, temp);
  ambient.send();
  
  // 1分停止
  delay(60000UL);
}
