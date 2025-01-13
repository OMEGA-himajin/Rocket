#include <Camera.h>
#include <SDHCI.h>
#include "AviLibrary.h"

SDClass theSD;
File aviFile;

AviLibrary theAvi;

String filename_base = "movie_";
uint32_t fileIndex = 0;  // ファイル名のインデックス（連番）

uint32_t lastTime = 0;   // 最後の録画時間
const uint32_t recording_interval = 10000; // 10秒ごとに分割

void CamCB(CamImage img) {
  if (img.isAvailable()) {
    uint32_t currentTime = millis();
    // 10秒ごとに録画を分割
    if (currentTime - lastTime >= recording_interval) {
      // 録画を終了
      theAvi.endRecording();
      theAvi.end();

      // 次のファイル名を設定
      String filename = filename_base + String(fileIndex++) + ".avi";
      aviFile = theSD.open(filename, FILE_WRITE);

      // 新しいファイルを開始
      theAvi.begin(aviFile, CAM_IMGSIZE_QVGA_H, CAM_IMGSIZE_QVGA_V);
      theAvi.startRecording();
      lastTime = currentTime;  // 時間のリセット
      Serial.println("New movie started: " + filename);
    }

    // フレームを追加
    theAvi.addFrame(img.getImgBuff(), img.getImgSize());
  }
}

void setup() {
  Serial.begin(115200);
  
  while (!theSD.begin()) { 
    Serial.println("Insert SD card"); 
    delay(1000);
  }

  const int buff_num = 2;
  theCamera.begin(buff_num, CAM_VIDEO_FPS_30, CAM_IMGSIZE_QVGA_H, CAM_IMGSIZE_QVGA_V, CAM_IMAGE_PIX_FMT_JPG, 5);

  // 最初のAVIファイルを作成
  String filename = filename_base + String(fileIndex++) + ".avi";
  aviFile = theSD.open(filename, FILE_WRITE);
  
  theAvi.begin(aviFile, CAM_IMGSIZE_QVGA_H, CAM_IMGSIZE_QVGA_V);
  theCamera.startStreaming(true, CamCB);

  // 録画開始
  theAvi.startRecording();
  lastTime = millis();  // 録画開始時間の設定
  Serial.println("Start recording...");
}

void loop() {
  // ループ内では特に処理は必要ない
}
