CC := gcc
CFLAGS := -fPIC -Wall -Wextra -Wpedantic -funsigned-char -std=c99
CPPFLAGS := -I/usr/include/libxml2
LDLIBS := -lxml2
RM := rm -rf
BINDIR := /usr/local/bin
LIBDIR := /usr/local/lib
MANDIR := /usr/local/share/man/man1

.PHONY: all install remove clean

all: dist/navi dist/libnavi.so

MODS_CLI := main
MODS_COMMON := cadena log
MODS_LIB := init gfonts

# src/common/*.o: src/common/*.c
$(addprefix src/common/,$(addsuffix .o,$(MODS_COMMON))): $(addprefix src/common/,$(addsuffix .c,$(MODS_COMMON)))
	@echo Generando $(notdir $@)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $^
	mv *.o src/common/

# src/cli/*.o: src/cli/*.c
$(addprefix src/cli/,$(addsuffix .o,$(MODS_CLI))): $(addprefix src/cli/,$(addsuffix .c,$(MODS_CLI)))
	@echo Generando $(notdir $@)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $^
	mv *.o src/cli/

# src/lib/*.o: src/lib/*.c
$(addprefix src/lib/,$(addsuffix .o,$(MODS_LIB))): $(addprefix src/lib/,$(addsuffix .c,$(MODS_LIB)))
	@echo Generando $(notdir $@)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $^
	mv *.o src/lib/

# dist/libnavi.so: src/lib/*.o src/common/*.o
dist/libnavi.so: $(addprefix src/lib/,$(addsuffix .o,$(MODS_LIB))) $(addprefix src/common/,$(addsuffix .o,$(MODS_COMMON)))
	@mkdir -p dist
	$(CC) -shared -fPIC $(LDFLAGS) $^ $(LDLIBS) -o $@

# dist/navi: src/cli/*.o
dist/navi: dist/libnavi.so $(addprefix src/cli/,$(addsuffix .o,$(MODS_CLI)))
	@mkdir -p dist
	$(CC) $(LDFLAGS) $^ -L dist -lnavi $(LDLIBS) -o $@

#po/fr/$(EXEC).mo: po/fr/$(EXEC).po
#    msgfmt --output-file=$@ $<

#po/fr/$(EXEC).po: po/$(EXEC).pot
#    msgmerge --update $@ $<

#src/cli/%.pot: src/cli/%.c
#	xgettext -k_ -j -lC -c -s -o $< $@.c

install:
	mv dist/navi $(BINDIR)
	mv dist/libnavi.so $(LIBDIR)

remove:
	$(RM) $(BINDIR)/navi
	$(RM) $(LIBDIR)/libnavi.so

clean:
	$(RM) dist/*
	find . -regex .*\\.o -delete
	find . -regex .*\\.gch -delete
	find . -regex .*\\.pot -delete
	find . -regex .*\\.mo -delete
