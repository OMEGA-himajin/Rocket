#include <SPI.h>
#include <SD.h>
#include <Camera.h>

void setup() {
  Serial.begin(115200);

  // SDカードの初期化
  if (!SD.begin(10)) {
    Serial.println("SDカードの初期化に失敗しました");
    while (1);
  }
  Serial.println("SDカードが初期化されました");

  // カメラの初期化
  if (!Camera.begin(CAM_IMAGE_SIZE_QVGA)) {
    Serial.println("カメラの初期化に失敗しました");
    while (1);
  }
  Camera.setAutoExposure(true);
  Serial.println("カメラが初期化されました");
}

void loop() {
  size_t frameSize;
  uint8_t* frameData = Camera.createFrame(&frameSize);

  if (frameData) {
    Serial.println("フレームデータを取得しました");
    // 必要に応じてSDカードに保存
    File file = SD.open("frame.jpg", FILE_WRITE);
    if (file) {
      file.write(frameData, frameSize);
      file.close();
      Serial.println("フレームをSDカードに保存しました");
    } else {
      Serial.println("SDカードへの書き込みに失敗しました");
    }
  }

  delay(1000); // 1秒間隔でループ
}
