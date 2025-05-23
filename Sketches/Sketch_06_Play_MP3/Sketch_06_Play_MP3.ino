/*
* Sketch_05_Play_MP3.ino
* This sketch plays an MP3 file from an SD card using the Audio library.
* It initializes the SD card and I2S interface, and plays a specified MP3 file.
* 
* Author: Zhentao Lin
* Date:   2025-04-07
*/

#include "Arduino.h"
#include "Audio.h"
#include "FS.h"
#include "SD_MMC.h"

#define SD_MMC_CMD 38  // Command pin for SDMMC
#define SD_MMC_CLK 39  // Clock pin for SDMMC
#define SD_MMC_D0  40  // Data pin for SDMMC

#define I2S_BCLK   42  // Bit clock pin for I2S
#define I2S_DOUT   1   // Data out pin for I2S
#define I2S_LRC    41  // Left/Right clock pin for I2S

Audio audio;

void setup() {
 Serial.begin(115200);
 
 SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);
 if (!SD_MMC.begin("/sdcard", true, true, SDMMC_FREQ_DEFAULT, 5)) {
   Serial.println("Card Mount Failed");
   return;
 }
 uint8_t cardType = SD_MMC.cardType();
 if (cardType == CARD_NONE) {
   Serial.println("No SD_MMC card attached");
   return;
 }
 if (cardType == CARD_MMC) {
   Serial.println("MMC");
 } else if (cardType == CARD_SD) {
   Serial.println("SDSC");
 } else if (cardType == CARD_SDHC) {
   Serial.println("SDHC");
 } else {
   Serial.println("UNKNOWN");
 }
 uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
 Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);

 audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
 audio.setVolume(2);  // 0...21

 audio.connecttoFS(SD_MMC, "/music/Jingle Bells.mp3");
}

void loop() {
 audio.loop();
 if (Serial.available()) {  // put streamURL in serial monitor
   audio.stopSong();
   String r = Serial.readString();
   r.trim();
   if (r.length() > 5) audio.connecttoFS(SD_MMC, r.c_str());
   log_i("free heap=%i", ESP.getFreeHeap());
 }
}

// optional
void audio_info(const char *info) {
 Serial.print("info        ");
 Serial.println(info);
}
void audio_id3data(const char *info) {  //id3 metadata
 Serial.print("id3data     ");
 Serial.println(info);
}
void audio_eof_mp3(const char *info) {  //end of file
 Serial.print("eof_mp3     ");
 Serial.println(info);
}
void audio_showstation(const char *info) {
 Serial.print("station     ");
 Serial.println(info);
}
void audio_showstreamtitle(const char *info) {
 Serial.print("streamtitle ");
 Serial.println(info);
}
void audio_bitrate(const char *info) {
 Serial.print("bitrate     ");
 Serial.println(info);
}
void audio_commercial(const char *info) {  //duration in sec
 Serial.print("commercial  ");
 Serial.println(info);
}
void audio_icyurl(const char *info) {  //homepage
 Serial.print("icyurl      ");
 Serial.println(info);
}
void audio_lasthost(const char *info) {  //stream URL played
 Serial.print("lasthost    ");
 Serial.println(info);
}