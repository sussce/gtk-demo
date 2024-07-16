CC ?= gcc
RES_CC ?= glib-compile-resources
SCHEMA_CC ?= glib-compile-schemas

cflags = `pkg-config --cflags gtk4`
ldflags = `pkg-config --libs gtk4`
includes =-Isrc


src = src/mystats/mystats.c src/myop/myop.c src/mycomment/mycomment.c src/myarea/myarea.c src/myinfo/myinfo.c src/mywidgetlayout/mywidgetlayout.c src/mywidget/mywidget.c src/mywindow/mywindow.c src/myapp/myapp.c src/main.c
src_res = res/gresource.c
res = res/gresource.xml

obj = $(src:.c=.o) $(src_res:.c=.o)
bin = app

.PHONY: all clean

all: $(bin)

clean:
	$(RM) $(obj)
	$(RM) $(bin)
	$(RM) $(src_res)

$(bin): $(obj)
	$(CC) $(ldflags) -o $@ $(obj)

$(src_res): $(res)
	$(RES_CC) $< --target=$@ --sourcedir=res --generate-source

%.o: %.c
	$(CC) $(cflags) $(includes) -c -o $@ $<
