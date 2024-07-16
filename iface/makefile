CC ?= GCC

cflags = `pkg-config --cflags gobject-2.0`
ldflags = `pkg-config --libs gobject-2.0`
include = -Iaeditable/ -Iafile/ -Iaaudio/

src = aeditable/aeditable.c afile/afile.c aaudio/aaudio.c main.c
src_afile = aeditable/aeditable.c afile/afile.c afile/main.c
obj = $(src:.c=.o)
obj_afile = $(src_afile:.c=.o)
bin = audio
bin_afile = file

.PHONY: all afile clean

all: $(bin)

afile: $(bin_afile)

clean:
	$(RM) $(obj) $(obj_afile)
	$(RM) $(bin) $(bin_afile)

$(bin): $(obj)
	$(CC) $(ldflags) -o $@ $(obj)

$(bin_afile): $(obj_afile)
	$(CC) $(ldflags) -o $@ $(obj_afile)

%.o: %.c
	$(CC) $(cflags) $(include) -c -o $@ $<

