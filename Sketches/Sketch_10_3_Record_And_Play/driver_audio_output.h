
#ifndef __DRIVER_AUDIO_OUTPUT_H
#define __DRIVER_AUDIO_OUTPUT_H

int audio_output_init(int bclk, int lrc, int dout);
void audio_output_set_volume(int volume);
int audio_read_output_volume(void);
void audio_output_load_music(const char *name);
void audio_output_pause_resume(void);
void audio_output_stop(void);
bool audio_output_is_running(void);
long audio_get_total_output_playing_time(void);
long audio_output_get_file_duration(void);
bool audio_output_set_play_position(int second);
long audio_read_output_play_position(void);
void audio_output_loop(void);
void audio_info(const char *info);
void audio_eof_mp3(const char *info);






#endif

