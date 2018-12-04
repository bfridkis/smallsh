CFLAGS = -g -Wall -Wfatal-errors -std=gnu99
#CFLAGS = -g -w -std=gnu99
VPATH = cutest

smallsh : smallsh.o dynarr.o loadArgs.o getCommand.o executeCMD.o sigToggle.o sigTSTPHandler.o
	gcc $(CFLAGS) -o $@ $^

smallsh.o : smallsh.c
	gcc $(CFLAGS) -c -o $@ $<
	
getCommand.o : getCommand.c
	gcc $(CFLAGS) -c -o $@ $<

dynarr.o : dynarr.c
	gcc $(CFLAGS) -c -o $@ $<
	
loadArgs.o : loadArgs.c
	gcc $(CFLAGS) -c -o $@ $<

executeCMD.o : executeCMD.c
	gcc $(CFLAGS) -c -o $@ $<

sigToggle.o : sigToggle.c
	gcc $(CFLAGS) -c -o $@ $<

sigTSTPHandler.o : sigTSTPHandler.c
	gcc $(CFLAGS) -c -o $@ $<

memcheckProg : smallsh
	valgrind --tool=memcheck --leak-check=yes smallsh

all : smallsh
	
run :
	./smallsh
	
clean :
	-rm *.o
	-rm smallsh
