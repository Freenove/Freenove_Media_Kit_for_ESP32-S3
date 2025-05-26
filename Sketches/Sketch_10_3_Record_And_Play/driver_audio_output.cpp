
#include "driver_audio_output.h"
#include "SD_MMC.h"
#include "Audio.h"
#include <ESP_I2S.h>

Audio audio;
I2SClass i2s_output; 

bool i2s_output_init(int bclk, int lrc, int dout) {
  i2s_output.setPins(bclk, lrc, dout, -1);
  if (!i2s_output.begin(I2S_MODE_STD, 16000, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO, I2S_STD_SLOT_RIGHT)) {
    Serial.println("Failed to initialize I2S output bus!");
    return false;
  }
  i2s_output.write(0); 
  i2s_output.write(0); 
  i2s_output.end();
  return true;
}

//Initialize the audio interface
int audio_output_init(int bclk, int lrc, int dout) {
  i2s_output_init(bclk, lrc, dout);
  return audio.setPinout(bclk, lrc, dout);
}

//Set the volume: 0-21
void audio_output_set_volume(int volume) {
  audio.setVolume(volume);
}

//Query volume
int audio_read_output_volume(void) {
  return audio.getVolume();
}

//load the mp3
void audio_output_load_music(const char *name) {
  audio.connecttoFS(SD_MMC, name);
}

//Pause/play the music
void audio_output_pause_resume(void) {
  audio.pauseResume();
}

//Stop the music
void audio_output_stop(void) {
  audio.stopSong();
}

//Whether the music is running
bool audio_output_is_running(void) {
  return audio.isRunning();
}

//Gets how long the music player has been playing
long audio_get_total_output_playing_time(void) {
  return (long)audio.getTotalPlayingTime() / 1000;
}

//Obtain the playing time of the music file
long audio_output_get_file_duration(void) {
  return (long)audio.getAudioFileDuration();
}

//Set play position
bool audio_output_set_play_position(int second) {
  return audio.setAudioPlayPosition((uint16_t)second);
}

//Gets the current playing time of the music
long audio_read_output_play_position(void) {
  return audio.getAudioCurrentTime();
}

//Non-blocking music execution function
void audio_output_loop(void) {
  audio.loop();
}

// optional
void audio_info(const char *info) {
  Serial.print("info        ");
  Serial.println(info);
}

void audio_eof_mp3(const char *info) {  
  Serial.print("eof_mp3     ");
  Serial.println(info);
}