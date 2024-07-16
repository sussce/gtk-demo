#ifndef _a_AUDIO_H_
#define _a_AUDIO_H_

#include <glib-object.h>
#include "./../afile/afile.h"

#define A_TYPE_AUDIO (a_audio_get_type())
G_DECLARE_FINAL_TYPE(AAudio, a_audio, A, AUDIO, AFile);
  
AAudio* a_audio_new();

#endif /* _A_AUDIO_H_ */
