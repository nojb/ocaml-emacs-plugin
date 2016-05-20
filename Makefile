ROOT = emacs/emacs-25.0.92

all: emacs_stubs.o lib/emacs_plugin.cmx test

test: test_plugin.so

emacs_stubs.o: lib/emacs_stubs.c
	gcc -std=c99 -Wall -I`ocamlc -where` -I$(ROOT)/src -fPIC -c lib/emacs_stubs.c -o emacs_stubs.o

lib/emacs_plugin.cmx lib/emacs_plugin.o: lib/emacs_plugin.mli lib/emacs_plugin.ml
	ocamlopt -verbose -bin-annot -I lib/ -c $^

test_plugin.so: emacs_stubs.o lib/emacs_plugin.cmx test/test_plugin.ml
	ocamlopt -verbose -runtime-variant _pic -output-obj -I lib/ -o $@ $^

.PHONY: clean
clean:
	rm -f *.o *.so *.cm* lib/*.o lib/*.cm* test/*.o test/*.cm*
