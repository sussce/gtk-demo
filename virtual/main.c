#include <glib-object.h>
#include "aaudio/aaudio.h"

int main(int argc, char** argv) {
  AAudio* audio = a_audio_new();

  a_file_open(A_FILE(audio), NULL);
  a_file_close(A_FILE(audio), NULL);
  
  g_object_unref(audio);
  return 0;
}
