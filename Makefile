LIBS=../tcp-mini/libtcp-mini.lib

# non configurable..
DIR_PER_LIB=$(dir $(LIBS))
NOTDIR_PER_LIB=$(patsubst lib%.lib,%,$(notdir $(LIBS)))

tcp-mini-test.exe : main.o $(LIBS)
	gcc -o tcp-mini-test.exe main.o -lws2_32 $(addprefix -L,$(DIR_PER_LIB)) $(addprefix -l,$(NOTDIR_PER_LIB))

# always make..
.PHONY: $(LIBS)
$(LIBS):
	make -C $(dir $@)

main.o: main.c
	gcc -c main.c -I../tcp-mini/

#******************************************************************************

andrun: tcp-mini-test.exe
	./tcp-mini-test.exe

clean:
	if exist main.o del main.o
	if exist tcp-mini-test.exe del tcp-mini-test.exe
# if linux leftover.. also remove linux leftover..
	if exist tcp-mini-test del tcp-mini-test