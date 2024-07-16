#ifndef _A_AUDIO_H_
#define _A_AUDIO_H_

#include <glib-object.h>
#include "afile.h"
#include "aeditable.h"

#define A_TYPE_AUDIO (a_audio_get_type())
G_DECLARE_FINAL_TYPE(AAudio, a_audio, A, AUDIO, AFile);

#define A_AUDIO_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), A_TYPE_AUDIO, AAudioClass))

AAudio* a_audio_new();
void a_audio_open(AAudio* audio, GError** error);
void a_audio_save(AAudio* audio, GError** error);
void a_audio_changed(AEditable* editable, gpointer data);

#endif /* _A_AUDIO_H_ */
