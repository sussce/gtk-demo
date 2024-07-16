#include <glib-object.h>
#include "aaudio.h"
#include "aeditable.h"

int main(int argc, char** argv) {
  AAudio* audio = a_audio_new();

  g_signal_connect(A_EDITABLE(audio), "changed",
                   G_CALLBACK(a_audio_changed), NULL);

  
  //a_file_open(A_FILE(audio), NULL);
  //a_file_save(A_FILE(audio), NULL);

  //a_audio_open(audio, NULL);
  a_audio_save(audio, NULL);

  
  g_object_unref(audio);
  return 0;
}
