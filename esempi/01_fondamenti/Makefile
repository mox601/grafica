CFLAGS=
INCDIR=
LINKLIBS=-lglut -lGLU -lGL  -lm

all:opengl1
	@echo "all done"
	
.c.o:
	$(CC)  -w -o $@  -c $(CFLAGS) $(INCDIR) $<

opengl1: opengl1.o
	$(CC) -o $@ opengl1.o $(LINKLIBS)


clean:
	rm -f *.o  opengl1 
	@echo "clean done"


