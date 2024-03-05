CC := gcc
CFLAGS := -fPIC -Wall -Wextra -Wpedantic -std=c99

all: dist/navi dist/libnavi.so

src/common/%.o: src/common/%.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $^ -o $@

src/cli/%.o: src/cli/%.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $^ -o $@

src/lib/%.o: src/lib/%.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $^ -o $@

dist/libnavi.so: src/lib/%.o src/common/%.o
	$(CC) -shared $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

dist/navi: src/cli/%.o src/common/%.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

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

src/cli/%.pot: src/cli/%.c
	xgettext -k_ -j -lC -c -s -o $< $@.c

.PHONY: clean
