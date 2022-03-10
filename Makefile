LIBS=../tcp-mini/libtcp-mini.a

# non configurable..
DIR_PER_LIB=$(dir $(LIBS))
NOTDIR_PER_LIB=$(patsubst lib%.a,%,$(notdir $(LIBS)))

tcp-mini-test : main.o $(LIBS)
	gcc -o tcp-mini-test main.o -lpthread $(addprefix -L,$(DIR_PER_LIB)) $(addprefix -l,$(NOTDIR_PER_LIB))

# always make..
.PHONY: $(LIBS)
$(LIBS):
	make -C $(dir $@)

main.o: main.c
	gcc -c main.c -I../tcp-mini/

#******************************************************************************

andrun: tcp-mini-test
	./tcp-mini-test

clean:
	rm -f main.o
	rm -f tcp-mini-test
# if windows leftover.. also remove windows leftover..
	rm -f tcp-mini-test.exe

