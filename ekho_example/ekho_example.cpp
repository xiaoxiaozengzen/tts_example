#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include <sndfile.h>

#include "config.h" // this file is generated when building Ekho
#include "ekho.h"

int main() {
  ekho::Ekho ekho_player;
  ekho_player.setVoice("Mandarin");  
  ekho_player.setChannels(2);
  ekho_player.setSampleRate(44100);

  // output text to WAVE file
  ekho_player.saveWav("hahaha", "/mnt/workspace/cgz_workspace/Exercise/tts_example/output/hahaha.wav");

  return 0;
}