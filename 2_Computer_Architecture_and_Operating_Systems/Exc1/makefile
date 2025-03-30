CFLAGS=-Wall -Wextra -Wno-implicit-fallthrough -std=gnu17 -fPIC -O2
LDFLAGS=-shared -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=realloc \
   -Wl,--wrap=reallocarray -Wl,--wrap=free -Wl,--wrap=strdup -Wl,--wrap=strndup

.PHONY: all clean

all: libnand.so

nand.o: nand.c nand.h
	gcc $(CFLAGS) -c nand.c

memory_tests.o: memory_tests.c memory_tests.h
	gcc $(CFLAGS) -c memory_tests.c

libnand.so: nand.o memory_tests.o
	gcc $(LDFLAGS) -o $@ $^

testy: testy.c libnand.so
	gcc $(CFLAGS) testy.c -L./ -lnand -o $@

nand_example: nand_example.c libnand.so
	gcc $(CFLAGS) nand_example.c -L./ -lnand -o $@

clean:
	rm -rf *.so *.o testy nand_example
