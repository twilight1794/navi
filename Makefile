CC := gcc
CFLAGS := -fPIC -Wall -Wextra -Wpedantic -funsigned-char -std=c99
LDFLAGS := -fPIC

.PHONY: all clean

#all: dist/navi dist/libnavi.so
all: dist/libnavi.so

MODS_COMMON := cadena log
MODS_LIB := gfonts

$(addprefix src/common/,$(addsuffix .o,$(MODS_COMMON))): $(addprefix src/common/,$(addsuffix .c,$(MODS_COMMON)))
	@echo Generando $(notdir $@)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $^
	mv *.o src/common/

#src/cli/%.o: src/cli/%.c
#	$(CC) -c $(CPPFLAGS) $(CFLAGS) $^ -o $@

#src/lib/%.o: src/lib/%.c
#	$(CC) -c $(CPPFLAGS) $(CFLAGS) $^ -o $@

#dist/libnavi.so: src/lib/*.o src/common/*.o
dist/libnavi.so: $(addprefix src/common/,$(addsuffix .o,$(MODS_COMMON)))
	@mkdir -p dist
	$(CC) -shared $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

#dist/navi: src/cli/*.o src/common/*.o
#	@mkdir -p dist
#	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	rm -rf dist/*
	find . -regex *.o -delete
	find . -regex *.gch -delete
	find . -regex *.pot -delete
	find . -regex *.mo -delete

#po/fr/$(EXEC).mo: po/fr/$(EXEC).po
#    msgfmt --output-file=$@ $<

#po/fr/$(EXEC).po: po/$(EXEC).pot
#    msgmerge --update $@ $<

#src/cli/%.pot: src/cli/%.c
#	xgettext -k_ -j -lC -c -s -o $< $@.c
