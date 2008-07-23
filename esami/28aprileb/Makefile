CFLAGS=
INCDIR=
#linux#LINKLIBS=-lglut -lGLU -lGL -lm

LINKLIBS=-framework OpenGL -framework Glut -lm


OBJECTS= esame2008

all: $(OBJECTS)
	@echo "all done"
	
esame2008: esame2008.o
	$(CC) -o $@ $^ $(LINKLIBS)

clean:
	rm -f *.o $(OBJECTS) *~
	@echo "clean done"
	
	
