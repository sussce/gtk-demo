CC ?= GCC

cflags = `pkg-config --cflags gtk4`
ldflags = `pkg-config --libs gtk4`
include = 

src = mywidget.c main.c
obj = $(src:.c=.o)
bin = widget

.PHONY: all clean

all: $(bin)

clean:
	$(RM) $(obj)
	$(RM) $(bin)

$(bin): $(obj)
	$(CC) $(ldflags) -o $@ $(obj)

%.o: %.c
	$(CC) $(cflags) $(include) -c -o $@ $<
