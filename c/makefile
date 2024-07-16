CC ?= gcc
RES_CC ?= glib-compile-resources

cflags = `pkg-config --cflags gtk4`
ldflags = `pkg-config --libs gtk4`
include =

src = main.c
obj = $(src:.c=.o)
bin = main

all: $(bin)

clean:
	$(RM) $(obj)
	$(RM) $(bin)

$(bin): $(obj)
	$(CC) $(ldflags) -o $@ $(obj)

%.o: %.c
	$(CC) $(cflags) -c -o $@ $<
